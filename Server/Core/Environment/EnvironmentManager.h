#pragma once

#include "..//Parser/CConfigParser.h"

//TODO : 싱글톤화 시키기 ( 싱글톤 클래스 만들어서 상속하면 싱글톤 되게 생성하는 방식으로 )
class EnvironmentManager
	: public CConfigParser
{
public:

	EnvironmentManager();

private:


};

