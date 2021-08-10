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

			if ( key[0] == '#' )
				continue;

			_table[ key ] = value;
		}

		openFile.close();
	}
}

bool CConfigParser::Contain( const std::string& key )
{
	if ( _table.find( key ) == _table.end() )
		return false;

	return true;
}

bool CConfigParser::GetBool( const std::string& key )
{
	if ( Contain( key ) ) 
	{
		if ( _table[ key ][ 0 ] == 't' || _table[ key ][ 0 ] == 'T' ) 
			return true;

		else 
			return false;
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}

std::string CConfigParser::GetString( const std::string& key )
{
	if ( Contain( key ) ) 
	{
		if ( _table[ key ].find( "\"" ) == std::string::npos ) 
			return _table[ key ];

		else 
			return _table[ key ].substr( 1, _table[ key ].length() - 2 );
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}

float CConfigParser::GetFloat( const std::string& key )
{
	if ( Contain( key ) ) 
	{
		return std::stof( _table[ key ] );
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}

int CConfigParser::GetInt( const std::string& key )
{
	if ( Contain( key ) )
	{
		return std::stoi( _table[ key ] );
	}
	else 
	{
		throw std::invalid_argument( "Not exist Name." );
	}
}