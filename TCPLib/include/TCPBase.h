#pragma once

// WinSock2 Windows Sockets
#define WIN32_LEAN_AND_MEAN

#include "common.h"
#include "Buffer.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

class TCPBase {
protected:
    SOCKET m_serverSocket;
    addrinfo* m_pInfo;

    // Deals with any socket error responses from the WSA
    void m_SocketError(const char* function, SOCKET socket);

    // Deals with any results error responses from WSA
    void m_ResultError(const char* function, int result);

    // All wsa is initialized and socket created
    bool m_isInitialized;

public:
    // ctors & dtors
    TCPBase();
    ~TCPBase();

    // Getters
    const SOCKET& GetSocket();

    // Initializes WSA, addres info and socket
    bool Initialize(const char* host, const char* port);
    // Release and delete all resources
    void Destroy();

    // Serializes and send message to the destination socket
    void SendRequest(const SOCKET& destSocket, const MSG_TYPE& msgType, uint32 idRoom, const std::string& msg);

    // Serializes message into buffer, accordingly to the message type (0 - action, 1 - chat message)
    void SerializeBuffer(const MSG_TYPE& msgType, uint32 idRoom, const std::string& msg, Buffer& bufferOut, int& packetSizeOut);

    // Deserializes message received from socket and returns the message type and string
    void ReceiveRequest(const SOCKET& origSocket, MSG_TYPE& msgTypeOut, uint32 idRoom, std::string& msgOut);

    // Deserialize message from buffer returning the message type and string
    uint32 DeserializeBuffer(Buffer& buffer, MSG_TYPE& msgTypeOut, uint32 idRoom, std::string& msgOut);
};