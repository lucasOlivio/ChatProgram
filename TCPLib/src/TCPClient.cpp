#include "TCPClient.h"

TCPClient::TCPClient()
{
    // For clients we can't stay waiting new msgs for too long
    this->m_tv.tv_sec = 0;
    this->m_tv.tv_usec = 0.1;
}

TCPClient::~TCPClient()
{
}

bool TCPClient::Initialize(const char* host, const char* port)
{
    bool TCPInitialized = TCPBase::Initialize(host, port);
    if (!TCPInitialized)
    {
        // Error initializing sockets
        return false;
    }

    // Try to connect to server
    bool isConnected = this->Connect();
    if (!isConnected)
    {
        // Not able to connect to server
        return false;
    }

    return true;
}

void TCPClient::Destroy()
{
    TCPBase::Destroy();
    return;
}

bool TCPClient::Connect()
{
    connect(this->m_serverSocket, this->m_pInfo->ai_addr, (int)this->m_pInfo->ai_addrlen);
    if (this->m_serverSocket == INVALID_SOCKET) {
        this->m_SocketError("connect", this->m_serverSocket, true);
        return false;
    }
    printf("Connect to the server successfully!\n");
    return true;
}

bool TCPClient::CheckMsgFromServer()
{
    FD_SET socketsToRead;

    // Resets sockets to read
    FD_ZERO(&socketsToRead);

    // Add our server socket to our set to check for new connections
    FD_SET(this->m_serverSocket, &socketsToRead);

    // Non-blocking way of checking if there is any new msg from the sockets
    int count = select(0, &socketsToRead, NULL, NULL, &this->m_tv);
    if (count == 0)
    {
        // Timevalue expired
        return false;
    }
    if (count == SOCKET_ERROR)
    {
        this->m_ResultError("select", count, false);
        return false;
    }

    return true;
}

bool TCPClient::CheckMsgFromServer(int tv_sec)
{
    timeval local_tv;
    local_tv.tv_sec = tv_sec;
    local_tv.tv_usec = 0;

    FD_SET socketsToRead;

    // Resets sockets to read
    FD_ZERO(&socketsToRead);

    // Add our server socket to our set to check for new connections
    FD_SET(this->m_serverSocket, &socketsToRead);

    // Non-blocking way of checking if there is any new msg from the sockets
    int count = select(0, &socketsToRead, NULL, NULL, &local_tv);
    if (count == 0)
    {
        // Timevalue expired
        return false;
    }
    if (count == SOCKET_ERROR)
    {
        this->m_ResultError("select", count, false);
        return false;
    }

    return true;
}
