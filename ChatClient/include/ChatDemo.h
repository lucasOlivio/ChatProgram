#pragma once

#include "ChatClient.h"

class ChatDemo
{
private:
	ChatClient* m_pChat;

	// Room messages cache, cleared every time we leave or enter a room
	std::vector<std::string> m_roomMessages;

	// Message been typed by the user
	std::string m_myMsg;

	bool m_isRunning;
public:
	// Ctors & dtors
	ChatDemo();
	~ChatDemo();

	// Initialize and destroy client
	bool Initialize();
	void Destroy();

	// Run main loop
	void RunChat();

	// UI for user not in room
	void OutRoomScene();

	// Print all messages in cache
	void PrintAllMsgs();

	// Redraw all UI to update some in room scene info
	void RedrawInRoomScene();

	// UI for user in room
	void InRoomScene();
};