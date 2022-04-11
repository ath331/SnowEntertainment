#pragma once
#include "DBConnection.h"

class DBConnectionPool
{
public:
	DBConnectionPool();
	~DBConnectionPool();

	bool          Connect( int32 connectionCount, const WCHAR* connectionString );
	void          Clear();

	DBConnection* Pop();
	void          Push( DBConnection* connection );

private:
	USE_LOCK;
	SQLHENV _enviorment = SQL_NULL_HANDLE;
	Vector<DBConnection*> _connections;

};
