#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"
#include "JobTimer.h"
#include "DBConnectionPool.h"
#include "ConsoleLog.h"

Memory*				GMemory            = nullptr;
JobTimer*           GJobTimer          = nullptr;
GlobalQueue*        GGlobalQueue       = nullptr;
ConsoleLog*         GConsoleLogger     = nullptr;
ThreadManager*		GThreadManager     = nullptr;
DeadLockProfiler*	GDeadLockProfiler  = nullptr;
DBConnectionPool*   GDBConnectionPool  = nullptr;
SendBufferManager*	GSendBufferManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager     = new ThreadManager();
		GMemory            = new Memory();
		GSendBufferManager = new SendBufferManager();
		GDeadLockProfiler  = new DeadLockProfiler();
		GGlobalQueue       = new GlobalQueue();
		GJobTimer          = new JobTimer();
		GDBConnectionPool  = new DBConnectionPool();
		GConsoleLogger     = new ConsoleLog();
		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemory;
		delete GSendBufferManager;
		delete GDeadLockProfiler;
		delete GGlobalQueue;
		delete GJobTimer;
		delete GDBConnectionPool;
		delete GConsoleLogger;

		SocketUtils::Clear();
	}
} GCoreGlobal;