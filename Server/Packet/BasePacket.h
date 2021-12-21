#pragma once

#pragma warning(disable:4624) //가상 소멸자 암시적 삭제 경고 제거용
#include <memory>

#include "PktConsts.h"
#include "Header.h"

class BasePacket
{
public:
	BasePacket();
	~BasePacket();

	PacketHeader header;

protected:
	EResultCode _resultCode;

protected:
	virtual void OnPacketHandler() = 0;

	void SetResultCode( EResultCode resultCode = EResultCode::Max ) { _resultCode = resultCode; }
	EResultCode GetResultCode() { return _resultCode; }

public:
	void _SetPacketName( std::string name ) { header.packetName = name; }
	std::string _GetPacketName() { return header.packetName; }
};


typedef std::shared_ptr< BasePacket > BasePacketPtr;