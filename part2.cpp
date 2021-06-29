#include <iostream>
#include <cmath>
#include <sstream>
#include <cstdio>
#include <windows.h>
#include <chrono>



typedef std::chrono::high_resolution_clock Clock;
using namespace std;
double PCFreq = 0.0;
__int64 CounterStart = 0;

int signum(long long a)
{
	return (a > 0) - (a < 0);
}
//pomocnicze bo nie ogarnalem robienia tego na bitach w int


//funkcje znajdujace b na podstawie rozdzialu 2 i 3
unsigned long long rtzone(unsigned long long a, unsigned long long n, unsigned long long k, unsigned long long divisor)
{
	long long b, bstart, bend;
	long long ksq = pow(2, k);
	long long deno = pow(2, n) / divisor;
	long long mid =  a * divisor - ksq;



	//wyznaczanie b min i b max



	bstart = -(mid * deno) +1;
	//cout << "bstart: " <<bstart<< endl;
	bend = a - mid;
	//cout <<"bend: "<< bend << endl;
	b = (bstart + bend)/2;


	//cout << "b: " << b << endl;
	// wyznaczanie wagi hamminga i szukanie b z najmniejsza wag¹ hamminga
	//b = hamminweight(bstart, bend);

	////wyswietlanie testy:
	//cout << "start: " << bstart << endl;
	//cout << "end: " << bend << endl;
	//cout << "ostateczne b: " << b << endl;

	//zwracanie finalnego b
	if (bstart <= bend)
		return bstart;
	else
		return -1; // to zamiast jakiegos skomplikowanego exception handling, jezeli b w rzeczywistosci mialoby miec taka wartosc to algorytm i tak nie zadziala
}



long long rtztwo(unsigned long long a, unsigned long long n, unsigned long long k, unsigned long long divisor)
{
	long long b, bstart, bend;
	long long ksq = pow(2, k);
	long long deno = pow(2, n) / divisor;
	long long mid = a * divisor - ksq;
	//wyznaczanie b min i b max
	long long c = a - mid * deno;
	//c += -signum(c);
	c--;
	bstart = c;
	long long templog = log2(abs(bstart));
	b = pow(2, templog) *signum(c);
	if (b < 0)
		b = b * 2;
	return bstart;

}


//
//int rtztwodebug(int a, int n, int k, int divisor)
//{
//	cout << "RZTTWO DEBUG: " << endl;
//	int b, bstart, bend;
//	int ksq = pow(2, k);
//	int deno = pow(2, n) / divisor;
//	cout << "deno: " << deno << endl;
//	int mid = a * divisor - ksq;
//	cout << "mid: " << mid << endl;
//	int c = a - mid * deno;
//	c--;
//	bstart = c;
//	cout << "bstart: " << bstart << endl;
//	int templog = log2(abs(bstart));
//	cout << "temp log: " << templog << endl;
//	b = pow(2, templog) *signum(c);
//	if (b < 0)
//		b = b *2;
//	cout << "b: " << b<<endl;
//	return b;
//
//}



int main()
{
	unsigned long long divisor = 47; //d, zawsze nieparzysty z zalozenia
	unsigned long long number;
	unsigned long long log_div = log2(divisor) + 1; // dla d=11 -> 4;
	//int a, b, k, n = 17; // dla divisor = 11, n <= 15 bo inaczej wychodzi siê poza zakres inta, ogolnie to (2^2n+log_div)/log_div<2^32, czyli n <16 dla dowolnego divisor
	unsigned long long result1, result2, sum = 0, diff;
	double timer = 0;
	double start;
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2, t3, t4;           // ticks
	double elapsedTime1, elapsedTime2;
	QueryPerformanceFrequency(&frequency);

	
	unsigned long long tabk[64];
	unsigned long long a, b;
	unsigned long long zakres;
	tabk[0] = 2;
	for (int k = 16; k < 64; k++)
	{
		for (int n = tabk[k-16]+1; n < 64; n++)
		{
			unsigned long long suma = 0;
			unsigned long long zakres = pow(2, n);
			for (unsigned long long d = 3; d < zakres / 2; d += 2)
			{
				//wyznaczanie a i b
				unsigned long long ksq = pow(2, k);
				a = ksq / d;
				if (a % 2 == 1) //a nieparzyste, nie ruszamy
				{

					b = rtzone(a, n, k, d);
					if (b == -1) // to sie nie powinno dziac normalnie
					{
						//cout << " a: "<<a<<" d: "<<d << endl;
					

						a += 1;
						b = rtztwo(a, n, k, d);
	
						//cout << "b after: " <<b<< endl;
					}

				}
				else
				{
					/*a += 1;
					b = rtztwo(a, n, k, d);*/

					a--;
					b = rtzone(a, n, k, d);
					if (b == -1) // to sie nie powinno dziac normalnie
					{
						//cout << " a: " << a << " d: " << d << endl;


						a +=1;
						b = rtzone(a, n, k, d);
						if (b == -1) // to sie nie powinno dziac normalnie
						{
							//cout << " a: " << a << " d: " << d << endl;


							a += 1;
							b = rtztwo(a, n, k, d);
						
							//cout << "b after: " <<b<< endl;
						}

					}

				}
				//testowanie a i b
				for (unsigned long long i = 2*d+1; i <zakres/2; i++)
				{
					number = i;
					result1 = (a*number + b) >> k;
					result2 = number / d;
					diff = result2 - result1;
					if (diff != 0)
					{
						suma += diff;
					
						/*cout << "result: " << result1 << " result2: " << result2<<endl;
						cout << "number: " << number << " a: " << a << " b: " << b << endl;
						cout << "k: " << k << " n: " << n << " d: " << d << " dif: " << diff << endl;
						rtztwodebug(a, n, k, d);
						cout << endl;*/
						break;
					}
					

				}

			}
			cout << "k: " << k << " n: " << n << " dif: " << suma << endl;
			if (suma != 0)
			{
				tabk[k - 15] = n - 1;
				break;
			}
			//cout << "k: " << k << " n: " << n << " dif: " << suma << endl;

		}
		cout << tabk[k - 15]<<endl;

	}



	return 0;
}




////Algorytm na k i d od n 
//int dif = k - n;
//int range = pow(2, dif);
//int list;
//for (int i = 0; i < range; i++)
//{
//	tab = divisors(pow(2, k) - 1 - i); // funkcja divisors ma zwracaæ tablcice ze wszystkimi dzielnikami
//	for (int j; j < tab.size(); j++)
//	{
//		if (tab[j] < pow(2, n - 1) && tab[j] % 2 == 1) //filtrowanie dzielnikow
//		{
//			list.append(tab[j]); //dodawanie do listy
//		}
//	}
//}
//for (int i = 0; i < list.size(); i++)
//{
//	if (k + n - int(log2(list[i])) < 32) // czy sie miesci 
//	{
//		return true;
//	}
//}