#pragma once

#include <string>
#include <map>

class CConfigParser {
public:
	CConfigParser() { /*TODO : Log로 기본 생성자 쓰면 안된다고 하기?*/};
	CConfigParser( const std::string& path );

	virtual ~CConfigParser() {};

public:

	bool IsSuccess() { return _table.size() != 0; }

	bool Contain  ( const std::string& name );
	bool GetBool  ( const std::string& name );

	int   GetInt  ( const std::string& name );
	float GetFloat( const std::string& name );

	std::string GetString( const std::string& name );

private:

	std::map< std::string, std::string > _table;
};