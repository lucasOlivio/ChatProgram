#pragma once

#include "TCPClient.h"

class ChatClient
{
private:
	uint32 m_idRoom;
	std::string m_username;
	TCPClient* m_pTCP;

	bool m_isInitialized;
public:
	// ctors & dtors
	ChatClient();
	~ChatClient();

	bool Initialize(const char* host, const char* port);
	void Destroy();

	// Uses our TCP lib to send a chat message to to server
	void SendChatMessage(const string& msg);
	
	// Send a request to join the room using the referenced username
	// In case of error returns the msg in "errorMsgOut"
	bool JoinRoom(uint32 idRoom, string username, string& errorMsgOut);

	// Remove user from room
	void LeaveRoom();
};