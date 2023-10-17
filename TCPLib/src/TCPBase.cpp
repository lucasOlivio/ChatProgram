#include "TCPBase.h"

const uint BUFFER_SIZE = 512;

void TCPBase::m_SocketError(const char* function, SOCKET socket, bool isFatalError)
{
	printf("%s failed with error %d\n", function, WSAGetLastError());

	if (isFatalError)
	{
		this->Destroy();
	}
	return;
}

void TCPBase::m_ResultError(const char* function, int result, bool isFatalError)
{
	printf("%s failed with error %d\n", function, result);

	if (isFatalError)
	{
		this->Destroy();
	}
	return;
}

TCPBase::TCPBase()
{
    this->m_serverSocket = INVALID_SOCKET;
    this->m_pInfo = nullptr;
	this->m_isInitialized = false;
	this->m_tv.tv_sec = 1;
	this->m_tv.tv_usec = 0;
}

TCPBase::~TCPBase()
{
}

SOCKET& TCPBase::GetSocket()
{
    return this->m_serverSocket;
}

bool TCPBase::Initialize(const char* host, const char* port)
{
	if (this->m_isInitialized)
	{
		// Already initialized
		return true;
	}

	// Initialize WinSock
	WSADATA wsaData;
	int result;

	// Set version 2.2 with MAKEWORD(2,2)
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		this->m_ResultError("WSAStartup", result, true);
		return false;
	}
	printf("WSAStartup successfully!\n");

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));	// ensure we don't have garbage data 
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// Stream
	hints.ai_protocol = IPPROTO_TCP;	// TCP
	hints.ai_flags = AI_PASSIVE;

	// Set all address info 
	result = getaddrinfo(host, port, &hints, &this->m_pInfo);
	if (result != 0) {
		this->m_ResultError("getaddrinfo", result, true);
		return false;
	}
	printf("getaddrinfo successfully!\n");

	// Creates new socket
	this->m_serverSocket = socket(this->m_pInfo->ai_family, this->m_pInfo->ai_socktype, this->m_pInfo->ai_protocol);
	if (this->m_serverSocket == INVALID_SOCKET) {
		this->m_SocketError("socket", this->m_serverSocket, true);
		return false;
	}
	printf("socket created successfully!\n");

	this->m_isInitialized = true;
    return true;
}

void TCPBase::Destroy()
{
	if (this->m_serverSocket != INVALID_SOCKET)
	{
		closesocket(this->m_serverSocket);
	}
	if (this->m_pInfo)
	{
		freeaddrinfo(this->m_pInfo);
	}
	WSACleanup();
	this->m_isInitialized = false;
	return;
}

void TCPBase::SendRequest(SOCKET& destSocket, const myTcp::MSG_TYPE& msgType, uint32 idRoom,
							const std::string& username, const std::string& msg)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	Buffer buffer(BUFFER_SIZE);
	int packetSize = 0;

	// Build buffer object to send
	this->SerializeBuffer(msgType, idRoom, username, msg, buffer, packetSize);

	// Send data and validate if succesfull
	int result = send(destSocket, (const char*)(&buffer.vecBufferData[0]), packetSize, 0);
	if (result == SOCKET_ERROR) {
		this->m_SocketError("send", result, false);
		return;
	}
	return;
}

void TCPBase::SerializeBuffer(const myTcp::MSG_TYPE& msgType, uint32 idRoom, const std::string& username,
								const std::string& msg, Buffer& bufferOut, int& packetSizeOut)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	sPacketData packet;

	// Build our packet
	packet.msg = msg;
	packet.msgLength = packet.msg.length();
	packet.header.msgType = msgType;
	packet.header.idRoom = idRoom;
	packet.header.usernameLength = username.length();
	packet.header.username = username;
	packet.header.packetSize =
		packet.msg.length()
		+ sizeof(packet.msgLength)		
		+ sizeof(packet.header.msgType)	
		+ sizeof(packet.header.idRoom)
		+ sizeof(packet.header.usernameLength)
		+ packet.header.username.length()
		+ sizeof(packet.header.packetSize);	

	// Write our packet to the buffer
	packetSizeOut = packet.header.packetSize;

	bufferOut.WriteUInt32LE(packet.header.packetSize);
	bufferOut.WriteUInt32LE(packet.header.msgType);
	bufferOut.WriteUInt32LE(packet.header.idRoom);

	bufferOut.WriteUInt32LE(packet.header.usernameLength);
	bufferOut.WriteString(packet.header.username);

	bufferOut.WriteUInt32LE(packet.msgLength);		
	bufferOut.WriteString(packet.msg);

	return;
}

void TCPBase::ReceiveRequest(SOCKET& origSocket, sPacketData& dataOut)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	// TODO: buffer data comming in parts
	// Get a buffer data from the required socket
	Buffer buffer(BUFFER_SIZE);
	int result = recv(origSocket, (char*)(&buffer.vecBufferData[0]), BUFFER_SIZE, 0);

	if (result == SOCKET_ERROR) {
		this->m_SocketError("recv", result, false);
		// Disconnects user to avoid error spam
		dataOut.header.packetSize = 0;
		return;
	}

	// Transform the data into our readable string
	this->DeserializeBuffer(buffer, dataOut);

	return;
}

void TCPBase::DeserializeBuffer(Buffer& buffer, sPacketData& dataOut)
{
	if (!this->m_isInitialized)
	{
		return;
	}

	// Get header
	dataOut.header.packetSize = buffer.ReadUInt32LE();
	dataOut.header.msgType = buffer.ReadUInt32LE();
	dataOut.header.idRoom = buffer.ReadUInt32LE();

	dataOut.header.usernameLength = buffer.ReadUInt32LE();
	dataOut.header.username = buffer.ReadString(dataOut.header.usernameLength);

	// Get message string
	dataOut.msgLength = buffer.ReadUInt32LE();
	dataOut.msg = buffer.ReadString(dataOut.msgLength);

	return;
}
