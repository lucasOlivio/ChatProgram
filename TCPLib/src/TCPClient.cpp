#include "TCPClient.h"

TCPClient::TCPClient()
{
}

TCPClient::~TCPClient()
{
}

bool TCPClient::Initialize(const char* host, const char* port)
{
    bool TCPInitialized = TCPBase::Initialize(host, port);
    if (!TCPInitialized)
    {
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
        this->m_SocketError("connect", this->m_serverSocket);
        return false;
    }
    printf("Connect to the server successfully!\n");
    return true;
}
