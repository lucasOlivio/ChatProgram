#pragma once

#include "TCPBase.h"

class TCPClient : public TCPBase
{
public:
	// ctors & dtors
	TCPClient();
	virtual ~TCPClient();

	bool Initialize(const char* host, const char* port);
	void Destroy();

	bool Connect();
};