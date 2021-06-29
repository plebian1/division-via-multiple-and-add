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

int signum(int a)
{
	return (a > 0) - (a < 0);
}

int boolToInt(bool *tab)
{
	int val = 0;
	for (int i = 0; i < 32; i++)
	{
		if (tab[i])
		{
			val += pow(2, i);
		}
	}
	return val;

}

//chapter 4
int hamminweight(int start, int end) 
{	
	int weight = 0;
	bool bits[32];
	
	for (int i = 0; i < 32; i++)
	{
		bits[i] = false;
	}

	//cout << "bits: " << bits << endl;
	//cout << "bits: " << bits << endl;
	bool broken = false;
	for (int i = 31; i >= 0; i--)
	{
		//cout <<( (start >> i) & 1) << endl;

		if (((start >> i) & 1) == ((end >> i) & 1))
		{
			//cout << "bits1: " << bits << endl;
			bits[i] =	((start >> i) & 1);

			start -= pow(2, i) * ((start >> i) & 1);
			//cout <<"start"<< start<<endl;
		}
		else 
		{
			int log = log2(start) ;
			//cout << "bits: " << bits << endl;
			//cout << "start: " << start << endl;
			int temp = pow(2, log);
			if (temp < start)
			{
				temp *= 2;
				log++;
			}
			int temp2 = boolToInt(bits);
			temp2 += temp;
			weight = temp2;
			//cout << "bits: " << bits << endl;
			broken = true;
			break;
		}
	}

	if (!broken)
	{
		weight = boolToInt(bits);
	}
	/*cout << "weight: " << weight << endl;*/
	return weight;
}

//functions returning optimised b value, based on chapters 2 and 3
int rtzone(int a,int n,int k,int divisor)
{
	int b, bstart, bend ;
	int ksq = pow(2, k);
	int deno = pow(2, n) / divisor;
	int mid = ksq -  a * divisor ;
	// b min i b max
	bstart =  mid*deno;
	bend =ksq - a*(divisor-1) -1;
	//finding lowest hamming weight for provided b range
	b = hamminweight(bstart, bend);

	//wyswietlanie testy:
	/*cout << "start: " << bstart<<endl;
	cout << "end: " << bend<<endl;
	cout << "ostateczne b: " << b<<endl;
*/
	//zwracanie finalnego b
	return b;
}

int rtztwo(int a,  int n, int k, int divisor)
{
	int b, bstart, bend;
	int ksq = pow(2, k);
	int deno = pow(2, n) / divisor;
	int mid = a * divisor - ksq;
	//wyznaczanie b min i b max
	int c = a - mid * deno;
	//c += -signum(c);
	c--;
	bstart = c;
	int log = log2(bstart);
	b = pow(2, log);
	return b;

}

int rteone(int a,  int n, int k, int divisor)
{
	int b, bstart, bend;
	int ksq = pow(2, k);
	int deno = (pow(2, n + 1) + divisor - 3) / (2 * divisor);
	int mid = a * divisor - ksq;
	//wyznaczanie b min i b max
	bstart =  a*(divisor - 1)/2  + (ksq-a*divisor)* deno;
	
	bend =  a * ((divisor + 1) / 2) +ksq - a*divisor -1;

	//finding lowest hamming weight for provided b range

	b = hamminweight(bstart, bend);
	/*cout << "start: " << bstart << endl;
	cout << "end: " << bend << endl;
	cout << "ostateczne b: " << b << endl;*/
	//zwracanie finalnego b


	return b;

}

int rtetwo(int a,  int n, int k, int divisor)
{
	int b, bstart, bend;
	int ksq = pow(2, k);
	int deno = (pow(2, n+1) +divisor-1 )/ (2*divisor);
	int mid = a * divisor - ksq;
	//wyznaczanie b min i b max

	bstart = a * ((divisor -1) / 2) + ksq - a * divisor;

	bend = a * (divisor - 1) / 2 + (ksq - a * divisor)* deno -1;

	
	//finding lowest hamming weight for provided b range
	b = hamminweight(bstart, bend);

	/*cout << "start: " << bstart << endl;
	cout << "end: " << bend << endl;
	cout << "ostateczne b: " << b << endl;*/

	return b;

}

int rfone(int a,  int n, int k, int divisor)
{
	int b, bstart, bend;
	int ksq = pow(2, k);
	int deno = pow(2, n) / divisor;
	int mid = ksq - a * divisor ;

	
	bstart = mid *deno;
	bend = ksq-1;
	//finding lowest hamming weight for provided b range
	b = hamminweight(bstart, bend);

	//wyswietlanie testy:
	/*cout << "start: " << bstart << endl;
	cout << "end: " << bend << endl;
	cout << "ostateczne b: " << b << endl;*/
	
	return b;

}


int rftwo(int a,  int n, int k, int divisor)
{
	int b=0, bstart, bend;
	int ksq = pow(2, k);
	int deno = pow(2, n) / divisor;
	int mid = a * divisor - ksq;
	bstart = ksq - deno * mid;
	bstart--; //ostro mniejsze
	int log = log2(bstart);
	b = pow(2, log);

	return b;

}

int main()
{
	int divisor =99; //divisor 
	int number;
	int log_div =  log2(divisor)+1; // dla d=11 -> 4;
	int a, b, k, n =15; // setting n higher than 15 may result in program not working correctly cause of int limitations
	int result1,result2, sum = 0,diff;
	double timer = 0;
	double start;
	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2,t3,t4;           // ticks
	double elapsedTime1 , elapsedTime2;
	QueryPerformanceFrequency(&frequency);


	// powiekszenia wykladnika
	k = n + log_div; 
	int wykladnik_inc = log_div;
	int option = 1; //method of rounding, 1 is RTZ, 2 RTE, 3 RF
	
	//setting minimal k 
	//chapter 5
	if (option == 1)
	{
		int k1 = n+ wykladnik_inc+1, k2 =n + wykladnik_inc+1;
		for(int i =0; i<=log_div;i++)
		{
			int temp = pow(2, n)/divisor;
			if (pow(2, n + i) /(int(pow(-2, n + i) ) % divisor ) > divisor * temp - 1)
			{
				//cout << "here,  k = "<<n+i << endl;
				a = pow(2, n + i) / divisor;
				//cout << a<<endl;
				if (a % 2 == 0)
				{
					k1 = n + i;
					break;
				}
			}
			
		}

		for (int i = 0; i <= log_div; i++)
		{
			int temp = pow(2, n) / divisor;
			if (pow(2, n + i) / (int(pow(2, n + i)) % divisor) > divisor * temp -divisor+ 1)
			{
				a = pow(2, n + i) / divisor;
				if (a % 2 == 1)
				{
					k2 = n + i;
					break;
				}
			}

		}
		/*cout << "n: " << n << endl;
		cout << "k: " << k << endl;
		cout << "k1: " << k1 << endl;
		cout << "k2: " << k2 << endl;*/
		if (k1 < k2)
		{
			k = k1;
		}
		else
		{
			k = k2;
		}

	}
	//chapter 6
	else if (option == 2)
	{
		int k1 = n + wykladnik_inc + 1, k2 = n + wykladnik_inc + 1;
		for (int i = 0; i <= log_div; i++)
		{
			int temp = pow(2, n) / divisor;
			if (pow(2, n + i) / (int(pow(-2, n + i)) % divisor) > divisor * temp - 1)
			{
				//cout << "here,  k = "<<n+i << endl;
				a = pow(2, n + i) / divisor;
				//cout << a<<endl;
				if (a % 2 == 0)
				{
					k1 = n + i;
					break;
				}
			}

		}

		for (int i = 0; i <= log_div; i++)
		{
			int temp = pow(2, n) / divisor;
			if (pow(2, n + i) / (int(pow(2, n + i)) % divisor) > divisor * temp - divisor + 1)
			{
				a = pow(2, n + i) / divisor;
				if (a % 2 == 1)
				{
					k2 = n + i;
					break;
				}
			}

		}
		/*cout << "n: " << n << endl;
		cout << "k: " << k << endl;
		cout << "k1: " << k1 << endl;
		cout << "k2: " << k2 << endl;*/
		if (k1 < k2)
		{
			k = k1;
		}
		else
		{
			k = k2;
		}
		
	}
	else if (option == 3)
	{
		
		int k1 = n + wykladnik_inc + 1, k2 = n + wykladnik_inc + 1;
		for (int i = 0; i <= log_div; i++)
		{
			int temp = pow(2, n) / divisor;
			if (pow(2, n + i) / (int(pow(-2, n + i)) % divisor) >  temp )
			{
				//cout << "here,  k = "<<n+i << endl;
				a = pow(2, n + i) / divisor;
				//cout << a<<endl;
				if (a % 2 == 0)
				{
					k1 = n + i;
					break;
				}
			}

		}

		for (int i = 0; i <= log_div; i++)
		{
			int temp = pow(2, n) / divisor;
			if (pow(2, n + i) / (int(pow(2, n + i)) % divisor) >  temp )
			{
				a = pow(2, n + i) / divisor;
				if (a % 2 == 1)
				{
					k2 = n + i;
					break;
				}
			}

		}
		/*cout << "n: " << n << endl;
		cout << "k: " << k << endl;
		cout << "k1: " << k1 << endl;
		cout << "k2: " << k2 << endl;*/
		if (k1 < k2)
		{
			k = k1;
		}
		else
		{
			k = k2;
		}
	}


	//setting a,b
	int ksq = pow(2, k);
	a = ksq/divisor; 

	//int deno = pow(2, n) / divisor;
	int zakres = pow(2, n);
	if (a % 2 ==1) 
	{

		if (option == 1)//RTZ1
		{
			b = rtzone(a, n, k, divisor);
		}
		else if (option == 2)//RTE1
		{
			b = rteone(a, n, k, divisor);
		}
		else if (option == 3)//FR1
		{
			b = rfone(a, n, k, divisor);
		}

	}
	else
	{
		a++;
		b = 0;

		if (option == 1)//RTZ2
		{
			
			//b = rtztwo(a, n, k, divisor);
			b = 0;
		}
		else if (option == 2)//RTE2
		{

			b = rtetwo(a, n, k, divisor);
		}
		else if (option == 3)//FR2
		{
			b = 0;
			//b = rftwo(a, n, k, divisor);
		}
		
	}

	//tests, not essential part of algorithm


	QueryPerformanceCounter(&t1);
		sum = 0;
		for (int i = 1; i < zakres; i++)
		{
			number = i;
			result1 = (a*number + b) >> k;
			//result2 = number/divisor ;
			//diff = result2 - result1;
			/*sum += diff;
			if (diff != 0)
			{
				cout << number << " : " << result1 << " dif: " << diff << endl;
			}*/
		}

		//cout <<"b: " <<b << " a: " << a << " dif: " << sum << endl;

	QueryPerformanceCounter(&t2);
	elapsedTime1 = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
	cout << elapsedTime1 << endl;
	cout <<"b: " <<b << " a: " << a << " dif: " << sum << endl;


	QueryPerformanceCounter(&t3);
	for (int i = 0; i < zakres; i++)
	{
		number = i;
		result1 = number / divisor;
		//sum += result1;
		//cout <<number<<" : "<< result2<<endl;
	}
	QueryPerformanceCounter(&t4);
	elapsedTime2 = (t4.QuadPart - t3.QuadPart) * 1000.0 / frequency.QuadPart;
	cout << elapsedTime2 << endl;
	cout << sum;
	return 0;
}