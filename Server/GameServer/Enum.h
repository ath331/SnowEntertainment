#pragma once

#define BUF_SIZE 1024

enum class EIocpMode : short
{
	ACCEPT,
	RECV,
	SEND,

	MAX
};