#include<iostream>
using namespace std;
long factorial(int n);

int main()
{
	int n(0);
	cout<<"Input the number n\n";
	cin>>n;
	long val=factorial(n);
	cout<<"Calling the factorial function\n";
	cout<<"The calculated factorial is: "<<val<<'\n';
	cin.get();
	return 0;
}

long factorial(int n)
{
	long result(1);
	while(n > 0)
	{
		result*=n--;
	}
	return result;
}