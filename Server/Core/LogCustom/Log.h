#pragma once
#include <stdio.h>
#include <string>

#define COMMON_LOG( str ) CommonLog ( __FILE__, __LINE__, __FUNCTION__, str )
#define WARNING_LOG( str, y ) WarningLog( __FILE__, __LINE__, __FUNCTION__, str, y )
#define ERROR_LOG( str, y ) ErrorLog  ( __FILE__, __LINE__, __FUNCTION__, str, y )

#pragma warning(disable:26444)

class Log
{
public:
	Log() {} 
	virtual ~Log() {}

protected:
	virtual void _PrintLog( const char* filePath, int lineNum, const char* funcName, const char* msg, int errorNo ) = 0;

protected:
	std::string _SubStrFilePath( const char* filePath )
	{
		std::string filePathStr = filePath;
		size_t offset = filePathStr.rfind( "\\" );
		filePathStr = filePathStr.substr( offset + 1, filePathStr.length() );

		return filePathStr;
	}
};


class CommonLog :
	public Log
{
public:
	CommonLog( const char* filePath, int lineNum, const char* funcName, const char* msg, int errorNo = 0 )
	{
		_PrintLog( filePath, lineNum, funcName, msg, errorNo );
	}

	~CommonLog() {}

private:
	virtual void _PrintLog( const char* /*filePath*/, int /*lineNum*/, const char* /*funcName*/, const char* msg, int /*errorNo*/ )
	{
		printf( "[ Log ] %s \n", msg );
	}
};


class WarningLog :
	public Log
{
public:
	WarningLog( const char* filePath, int lineNum, const char* funcName, const char* msg, int errorNo )
	{
		_PrintLog( filePath, lineNum, funcName, msg, errorNo );
	}

	~WarningLog() {}

private:
	virtual void _PrintLog( const char* filePath, int lineNum, const char* funcName, const char* msg, int errorNo )
	{
		printf( "[ WARNING ] %s:%d, %s() Func \"%s\" ErrorNo : %d ", _SubStrFilePath( filePath ).c_str(), lineNum, funcName, msg, errorNo );
	}
};


class ErrorLog :
	public Log
{
public:
	ErrorLog( const char* filePath, int lineNum, const char* funcName, const char* msg, int errorNo )
	{
		_PrintLog( filePath, lineNum, funcName, msg, errorNo );
	}

	~ErrorLog() {}

private:
	virtual void _PrintLog( const char* filePath, int lineNum, const char* funcName, const char* msg, int errorNo )
	{
		printf( "[ ERROR ] %s:%d, %s() Func \"%s\" ErrorNo : %d ", _SubStrFilePath( filePath ).c_str(), lineNum, funcName, msg, errorNo );
		exit( 1 );
	}
};