#pragma once

#include "..//Parser/CConfigParser.h"
#include "..//SingleTon/SingleTonClass.h"

class EnvironmentManager : 
	public CConfigParser,
	public SingleTonClass< EnvironmentManager >
{
public:
	EnvironmentManager();
	~EnvironmentManager();

private:

};

