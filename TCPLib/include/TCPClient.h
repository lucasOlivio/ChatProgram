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

	// Confirms that exists a new msg from the server or not
	bool CheckMsgFromServer();
	// Set the amount of time in secodns to wait for a response
	bool CheckMsgFromServer(int tv_sec);
};