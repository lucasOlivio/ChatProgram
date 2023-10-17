#pragma once

#include "TCPClient.h"
#include <vector>

class ChatClient
{
private:
	uint32 m_idRoom;
	std::string m_username;

	bool m_isInitialized;

public:
	TCPClient* m_pTCP;

	// ctors & dtors
	ChatClient();
	~ChatClient();

	bool Initialize(const char* host, const char* port);
	void Destroy();

	// Check if user is in any room
	bool IsInRoom();

	// Check if still have connection to server
	bool IsOnline();

	// Uses our TCP lib to send a chat message to to server
	void SendChatMessage(const std::string& msg);
	
	// Send a request to join the room using the referenced username
	// In case of error returns the msg in "errorMsgOut"
	bool JoinRoom(uint32 idRoom, std::string username, std::string& errorMsgOut);

	// Remove user from room
	void LeaveRoom();

	// Receives all new messages for the room
	std::string ReceiveRoomMsg();
};