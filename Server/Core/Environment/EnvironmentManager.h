#pragma once

#include "..//Parser/CConfigParser.h"
#include "..//SingleTon/UxSingleTon.h"

class EnvironmentManager : 
	public CConfigParser,
	public UxSingleTon< EnvironmentManager >
{
public:

	EnvironmentManager();

private:


};

