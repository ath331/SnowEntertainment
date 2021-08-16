#pragma once
#include <stdio.h>
#include <string>
#include <winsock2.h>

#define COMMON_LOG( x ) CommonLog ( __FILE__, __LINE__, __FUNCTION__, x )
#define WARNING_LOG( x ) WarningLog( __FILE__, __LINE__, __FUNCTION__, x )
#define ERROR_LOG( x ) ErrorLog  ( __FILE__, __LINE__, __FUNCTION__, x )

class Log
{
public:
	Log() {} 
	virtual ~Log() {}

protected:
	virtual void _PrintLog( const char* filePath, int lineNum, const char* funcName, const char* msg ) = 0;

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
	CommonLog( const char* filePath, int lineNum, const char* funcName, const char* msg )
	{
		_PrintLog( filePath, lineNum, funcName, msg );
	}

	~CommonLog() {}

private:
	virtual void _PrintLog( const char* /*filePath*/, int /*lineNum*/, const char* /*funcName*/, const char* msg )
	{
		printf( "[ Log ] %s", msg );
	}
};


class WarningLog :
	public Log
{
public:
	WarningLog( const char* filePath, int lineNum, const char* funcName, const char* msg )
	{
		_PrintLog( filePath, lineNum, funcName, msg );
	}

	~WarningLog() {}

private:
	virtual void _PrintLog( const char* filePath, int lineNum, const char* funcName, const char* msg )
	{
		printf( "[ WARNING ] %s:%d, %s() Func \"%s\" ErrorNo : %d ", _SubStrFilePath( filePath ).c_str(), lineNum, funcName, msg, WSAGetLastError() );
	}
};


class ErrorLog :
	public Log
{
public:
	ErrorLog( const char* filePath, int lineNum, const char* funcName, const char* msg )
	{
		_PrintLog( filePath, lineNum, funcName, msg );
	}

	~ErrorLog() {}

private:
	virtual void _PrintLog( const char* filePath, int lineNum, const char* funcName, const char* msg )
	{
		printf( "[ ERROR ] %s:%d, %s() Func \"%s\" ErrorNo : %d ", _SubStrFilePath( filePath ).c_str(), lineNum, funcName, msg, WSAGetLastError() );
		exit( 1 );
	}
};