#pragma once

#include "TCPBase.h"

class TCPServer : public TCPBase
{
private:
	// Clients connected to server
	FD_SET m_activeSockets;
	// Sockets with msgs waiting to be read
	FD_SET m_socketsToRead;
	// Timeinterval to wait for new msgs
	timeval m_tv;

public:
	// ctors & dtors
	TCPServer();
	virtual ~TCPServer();

	bool Initialize(const char* host, const char* port);
	void Destroy();

	// Getters
	FD_SET& GetActiveSockets();

    // Bind address with socket and starts listening
    void StartListening();

	// Use select to check which sockets have msgs and update SET
	void UpdateSocketsToRead();

	// Check if there is any new connection and add to the active sockets SET
	void AddSocket();
};