#pragma once

#include "TCPServer.h"
#include <map>

class ChatServer
{
private:
	// Mapping to the rooms we have and the clients in each one
	map<uint32 /* idRoom */, map<std::string /*username*/, SOCKET>> m_mapRoomClients;

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
	bool IsUsernameAvailable(uint32 idRoom, const string& clientSocket);

	// Add client socket to room id key
	void AddClientToRoom(uint32 idRoom, const std::string& username, const SOCKET& client);

	// Remove client socket from room id key
	void RemoveClientFromRoom(uint32 idRoom, const std::string& username);

	// Send the given message to all clients inside the given room
	void BroadcastToRoom(uint32 idRoom, std::string username, const std::string& msg);
};