#pragma once

#include <string>
#include <map>

class CConfigParser {
public:
	CConfigParser( const std::string& path );

	virtual ~CConfigParser() {};

public:
	bool IsSuccess() { return _table.size() != 0; }

	bool Contain  ( const std::string& key );
	bool GetBool  ( const std::string& key );

	int   GetInt  ( const std::string& key );
	float GetFloat( const std::string& key );

	std::string GetString( const std::string& key );

private:
	std::map< std::string, std::string > _table;

};