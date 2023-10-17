#pragma once

#include "TCPServer.h"
#include <map>

class ChatServer
{
private:
	// Mapping to the rooms we have and the clients in each one
	std::map<uint32 /* idRoom */, std::map<std::string /*username*/, SOCKET>> m_mapRoomClients;

	// Our custom protocol
	TCPServer* m_pTCP;

	bool m_isInitialized;

public:
	ChatServer();
	~ChatServer();

	bool Initialize(const char* host, const char* port);

	void Destroy();

	bool IsRoomCreated(uint32 idRoom);

	// Check if this username is not already been used in this room
	bool IsUsernameAvailable(uint32 idRoom, const std::string& clientSocket);

	// Go through each room to check the socket
	void GetRoomAndUsernameBySocket(SOCKET& client, uint32& idRoomOut, std::string& usernameOut);

	// Add client socket to room id key
	void AddClientToRoom(uint32 idRoom, const std::string& username, SOCKET& client);

	// Remove client socket from room id key
	void RemoveClientFromRoom(uint32 idRoom, const std::string& username);

	// Send the given message to all clients inside the given room
	void BroadcastToRoom(uint32 idRoom, std::string username, const std::string& msg);

	// Get which clients sent message and decide what to do with it based on the message type:
	// ACTION will execute a server function.
	// CHAT_MESSAGE will broadcast the message to every client in the room
	void ExecuteIncommingMsgs();
};