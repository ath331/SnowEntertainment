#include <iostream>

using namespace std;

void SumNumber( int& a, int b )
{
	a += b;
}

int main()
{
	int a = 10;
	int b = 5;

	SumNumber( a, b );
	cout << a << endl;

	return 0;
}