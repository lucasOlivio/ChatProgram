#include "ChatServer.h"

ChatServer::ChatServer()
{
    this->m_mapRoomClients = {};

    this->m_pTCP = nullptr;
    this->m_isInitialized = false;
}

ChatServer::~ChatServer()
{
}

bool ChatServer::Initialize(const char* host, const char* port)
{
    if (this->m_isInitialized)
    {
        // Already initialized
        return true;
    }

    // Initialize WSA and socket
    this->m_pTCP = new TCPServer();
    bool TCPInitialized = this->m_pTCP->Initialize(host, port);
    if (!TCPInitialized)
    {
        printf("Error initializing TCP!");
        return false;
    }

    this->m_isInitialized = true;
    return true;
}

void ChatServer::Destroy()
{
    this->m_pTCP->Destroy();
    delete this->m_pTCP;
    this->m_isInitialized = false;
}

bool ChatServer::IsRoomCreated(uint32 idRoom)
{
    if (this->m_mapRoomClients.find(idRoom) == this->m_mapRoomClients.end())
    {
        return false;
    }
    return true;
}

bool ChatServer::IsUsernameAvailable(uint32 idRoom, const string& username)
{
    if ( !this->IsRoomCreated(idRoom) || this->m_mapRoomClients[idRoom].find(username) == this->m_mapRoomClients[idRoom].end())
    {
        return true;
    }
    return false;
}

void ChatServer::AddClientToRoom(uint32 idRoom, const std::string& username, const SOCKET& client)
{
    if (!this->m_isInitialized)
    {
        return;
    }

    if (idRoom == 0)
    {
        // Rooms should start from 1
        return;
    }

    if (!this->IsRoomCreated(idRoom))
    {
        // If room doesn't exist yet, create one empty
        this->m_mapRoomClients[idRoom] = {};
    }

    if (!this->IsUsernameAvailable(idRoom, username))
    {
        // Username not available, so return bad request
        this->m_pTCP->SendRequest(client, MSG_TYPE::RESPONSE, 0, "username already in use");
        return;
    }

    this->m_mapRoomClients[idRoom][username] = client;

    // Succesfully added the new user to the room
    this->BroadcastToRoom(idRoom, username, "has joined the room.");

    return;
}

void ChatServer::RemoveClientFromRoom(uint32 idRoom, const std::string& username)
{
    if (!this->m_isInitialized)
    {
        return;
    }

    map<std::string, SOCKET>::iterator it = this->m_mapRoomClients[idRoom].find(username);
    if (it == this->m_mapRoomClients[idRoom].end())
    {
        // User not in room
        return;
    }

    this->m_mapRoomClients[idRoom].erase(username);

    // Succesfully removed user from room
    this->BroadcastToRoom(idRoom, username, "has left the room.");

    return;
}

void ChatServer::BroadcastToRoom(uint32 idRoom, std::string username, const std::string& msg)
{
    if (!this->m_isInitialized)
    {
        return;
    }

    if (!this->IsRoomCreated(idRoom))
    {
        // If room doesn't exist yet, there is no one to send
        return;
    }

    std::string formatedMsg = "[" + username + "] " + msg;
    for (pair<std::string, SOCKET> user : this->m_mapRoomClients[idRoom])
    {
        this->m_pTCP->SendRequest(user.second, MSG_TYPE::CHAT_MESSAGE, idRoom, formatedMsg);
    }

    return;
}
