#include "TCPServer.h"

TCPServer::TCPServer()
{
    this->m_activeSockets = {};
    this->m_socketsToRead = {};
}

TCPServer::~TCPServer()
{
}

bool TCPServer::Initialize(const char* host, const char* port)
{
    bool TCPInitialized = TCPBase::Initialize(host, port);
    if (!TCPInitialized)
    {
        return false;
    }

    // Initialize the sets
    FD_ZERO(&this->m_activeSockets);
    FD_ZERO(&this->m_socketsToRead);

    // Start listening to connections
    this->StartListening();

    return true;
}

void TCPServer::Destroy()
{
    TCPBase::Destroy();
    return;
}

void TCPServer::StartListening()
{
	if (!this->m_isInitialized)
	{
		return;
	}

	int result;

	// Binds socket to address
	result = bind(this->m_serverSocket, this->m_pInfo->ai_addr, (int)this->m_pInfo->ai_addrlen);
	if (result == SOCKET_ERROR) {
		this->m_ResultError("bind", result, true);
		return;
	}
	printf("bind was successful!\n");

	// Start listening to any new connection
	result = listen(this->m_serverSocket, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		this->m_ResultError("listen", result, true);
		return;
	}
	printf("listen successful\n");

	return;
}

void TCPServer::UpdateSocketsToRead()
{
    if (!this->m_isInitialized)
    {
        return;
    }

    // Resets sockets to read
    FD_ZERO(&this->m_socketsToRead);

    // Add our server socket to our set to check for new connections
    FD_SET(this->m_serverSocket, &this->m_socketsToRead);

    // Add all of our active connections to our sockets to read
    for (uint i = 0; i < this->m_activeSockets.fd_count; i++)
    {
        FD_SET(this->m_activeSockets.fd_array[i], &this->m_socketsToRead);
    }

    // Non-blocking way of checking if there is any new msg from the sockets
    int count = select(0, &this->m_socketsToRead, NULL, NULL, &this->m_tv);
    if (count == 0)
    {
        // Timevalue expired
        return;
    }
    if (count == SOCKET_ERROR)
    {
        printf("select had an error %d\n", WSAGetLastError());
        return;
    }
}

void TCPServer::AddSocket()
{
	if (this->m_socketsToRead.fd_count <= 0 || !FD_ISSET(this->m_serverSocket, &this->m_socketsToRead))
	{
		// No new connections
		return;
	}

	// Set the incoming connection to a new socket
	SOCKET newConnection = accept(this->m_serverSocket, NULL, NULL);
	if (newConnection == INVALID_SOCKET)
	{
		// Handle errors
		printf("accept failed with error: %d\n", WSAGetLastError());
	}
	else
	{
		// Handle successful connection
		FD_SET(newConnection, &this->m_activeSockets);
		FD_CLR(this->m_serverSocket, &this->m_socketsToRead);

		printf("Client connected with Socket: %d\n", (int)newConnection);
	}
}

void TCPServer::ReadNewMsgs(std::map<SOCKET, sPacketData>& mapNewMsgsOut)
{
    // First use a non-blocking way to check which sockets have messages
    this->UpdateSocketsToRead();

    // Now loop through all the sockets with messages
    for (int i = 0; i < this->m_activeSockets.fd_count; i++)
    {
        SOCKET socket = this->m_activeSockets.fd_array[i];
        if (!FD_ISSET(socket, &this->m_socketsToRead))
        {
            // Socket doesn't have any msg
            continue;
        }

        // Get the user message, along with the room id he is in
        sPacketData* pPacketOut = new sPacketData();
        this->ReceiveRequest(socket, *pPacketOut);

        mapNewMsgsOut[socket] = *pPacketOut;

        if (pPacketOut->header.packetSize == 0)
        {
            printf("Client disconnected from socket %d...\n", (int)socket);
            FD_CLR(socket, &this->m_activeSockets);
        }

        FD_CLR(socket, &this->m_socketsToRead);
    }

    // Now if there is any messages remaining, they are to accept new connections
    this->AddSocket();
}
