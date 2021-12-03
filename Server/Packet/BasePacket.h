#pragma once

#pragma warning(disable:4624) //���� �Ҹ��� �Ͻ��� ���� ��� ���ſ�

#include "PktConsts.h"

#include "Header.h"

class BasePacket
{
public:
	BasePacket();
	~BasePacket();

	PakcetHeader header;

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


