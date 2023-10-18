#pragma once

// WinSock2 Windows Sockets
#define WIN32_LEAN_AND_MEAN

#include "PacketTypes.h"
#include "common.h"
#include "Buffer.h"
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// Include win32 lib
#pragma comment(lib, "Ws2_32.lib")

class TCPBase {
protected:
    SOCKET m_serverSocket;
    addrinfo* m_pInfo;
    // Timeinterval to wait for new msgs
    timeval m_tv;

    // Deals with any socket error responses from the WSA
    void m_SocketError(const char* function, SOCKET socket, bool isFatalError);

    // Deals with any results error responses from WSA
    void m_ResultError(const char* function, int result, bool isFatalError);

    // Serializes message into buffer, accordingly to the message type (0 - action, 1 - chat message)
    void m_SerializeBuffer(const myTcp::MSG_TYPE& msgType, uint32 idRoom, const std::string& username,
        const std::string& msg, Buffer& bufferOut);

    // Deserialize message from buffer returning the message type and string
    // returns: the packet size
    void m_DeserializeBuffer(Buffer& buffer, sPacketData& dataOut);

    // All wsa is initialized and socket created
    bool m_isInitialized;

public:
    // ctors & dtors
    TCPBase();
    ~TCPBase();

    // Getters
    SOCKET& GetSocket();

    // Initializes WSA, addres info and socket
    bool Initialize(const char* host, const char* port);
    // Release and delete all resources
    void Destroy();

    // Serializes and send message to the destination socket
    void SendRequest(SOCKET& destSocket, const myTcp::MSG_TYPE& msgType, uint32 idRoom, 
                        const std::string& username, const std::string& msg);

    // Deserializes message received from socket and returns the message type and string
    // returns: the packet size
    void ReceiveRequest(SOCKET& origSocket, sPacketData& dataOut);
};