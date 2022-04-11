#include "pch.h"
#include "DBConnectionPool.h"

DBConnectionPool::DBConnectionPool()
{
}

DBConnectionPool::~DBConnectionPool()
{
	Clear();
}

bool DBConnectionPool::Connect( int32 connectionCount, const WCHAR* connectionString )
{
	WRITE_LOCK;

	if ( ::SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_enviorment ) != SQL_SUCCESS )
		return false;

	if ( ::SQLSetEnvAttr( _enviorment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>( SQL_OV_ODBC3 ), 0 ) != SQL_SUCCESS )
		return false;	

	for ( int32 i = 0 ; i < connectionCount; i++ )
	{
		DBConnection* connection = xnew<DBConnection>();
		if ( !connection->Connect( _enviorment, connectionString ) )
			return false;


		_connections.push_back( connection );
	}

	return true;
}

void DBConnectionPool::Clear()
{
	WRITE_LOCK;

	if ( _enviorment != SQL_NULL_HANDLE )
	{
		::SQLFreeHandle( SQL_HANDLE_ENV, _enviorment );
		_enviorment = SQL_NULL_HANDLE;
	}

	for ( DBConnection* connection : _connections )
		xdelete( connection );

	_connections.clear();
}

DBConnection* DBConnectionPool::Pop()
{
	WRITE_LOCK;

	if ( _connections.empty() )
		return nullptr;

	DBConnection* connection = _connections.back();
	_connections.pop_back();

	return connection;
}

void DBConnectionPool::Push( DBConnection* connection )
{
	WRITE_LOCK;

	_connections.push_back( connection );
}
