#include <iostream>
#include "Parser/CConfigParser.h"

using namespace std;

int main()
{
	CConfigParser parser("../Ini/Environment.ini");

	cout << parser.GetString( "IP"        ) << endl;
	cout << parser.GetInt   ( "TestInt"   ) << endl;
	cout << parser.GetBool  ( "TestBool"  ) << endl;
	cout << parser.GetFloat ( "TestFloat" ) << endl;

	return 0;
}