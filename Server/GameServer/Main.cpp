#include <iostream>
#include "Environment/EnvironmentManager.h"

using namespace std;

int main()
{
	EnvironmentManager environmentManager;
	EnvironmentManager environmentManager2;

	cout << environmentManager.GetInt   ( "TestInt"   ) << endl;

	return 0;
}