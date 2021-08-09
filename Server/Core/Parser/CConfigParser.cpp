#include "CConfigParser.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

CConfigParser::CConfigParser( const std::string& path )
{
	// read File
	std::ifstream openFile( path );
	if ( openFile.is_open() ) 
	{
		std::string line;

		while ( getline( openFile, line ) ) 
		{
			std::string delimiter = " = ";
			if ( std::string::npos == line.find( delimiter ) ) 
				delimiter = "=";

			std::string key   = line.substr( 0, line.find( delimiter ) );
			std::string value = line.substr( line.find( delimiter ) + delimiter.length(), line.length() );

			_table[ key ] = value;
		}

		openFile.close();
	}
}

bool CConfigParser::Contain( const std::string& name )
{
	if ( _table.find( name ) == _table.end() ) 
		return false;

	return true;
}

bool CConfigParser::GetBool( const std::string& name )
{
	if ( Contain( name ) ) 
	{
		if ( _table[ name ][ 0 ] == 't' || _table[ name ][ 0 ] == 'T' ) 
			return true;

		else 
			return false;
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}

std::string CConfigParser::GetString( const std::string& name )
{
	if ( Contain( name ) ) 
	{
		if ( _table[ name ].find( "\"" ) == std::string::npos ) 
			return _table[ name ];

		else 
			return _table[ name ].substr( 1, _table[ name ].length() - 2 );
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}

float CConfigParser::GetFloat( const std::string& name )
{
	if ( Contain( name ) ) 
	{
		return std::stof( _table[ name ] );
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}

int CConfigParser::GetInt( const std::string& name )
{
	if ( Contain( name ) ) 
	{
		return std::stoi( _table[ name ] );
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}