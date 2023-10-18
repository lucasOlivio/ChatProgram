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

bool ChatServer::IsUsernameAvailable(uint32 idRoom, const std::string& username)
{
    if ( !this->IsRoomCreated(idRoom) || this->m_mapRoomClients[idRoom].find(username) == this->m_mapRoomClients[idRoom].end())
    {
        return true;
    }
    return false;
}

void ChatServer::GetRoomAndUsernameBySocket(SOCKET& client, uint32& idRoomOut, std::string& usernameOut)
{
    // We go through each room
    for (std::pair<uint32 /* idRoom */, std::map<std::string /*username*/, SOCKET>> room : this->m_mapRoomClients)
    {
        // We go through each user
        for (std::pair<std::string, SOCKET> user : room.second)
        {
            if (user.second == client)
            {
                // Found the user!
                idRoomOut = room.first;
                usernameOut = user.first;
                return;
            }
        }
    }
}

void ChatServer::AddClientToRoom(uint32 idRoom, const std::string& username, SOCKET& client)
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
        this->m_pTCP->SendRequest(client, myTcp::MSG_TYPE::RESPONSE, 0, "", "username already in use");
        return;
    }

    this->m_mapRoomClients[idRoom][username] = client;

    // Succesfully added the new user to the room
    this->m_pTCP->SendRequest(client, myTcp::MSG_TYPE::RESPONSE, 0, "", "ok");
    std::string joinedMsg = "has joined the room";
    this->BroadcastToRoom(idRoom, username, joinedMsg);

    printf("%d %s %d\n", (int)client, joinedMsg.c_str(), idRoom);

    return;
}

void ChatServer::RemoveClientFromRoom(uint32 idRoom, const std::string& username)
{
    if (!this->m_isInitialized)
    {
        return;
    }

    // Try to find user in room by his username
    std::map<std::string, SOCKET>::iterator it = this->m_mapRoomClients[idRoom].find(username);
    if (it == this->m_mapRoomClients[idRoom].end())
    {
        // User not in room
        return;
    }

    // Found user in room so we can remove him
    std::string leftMsg = "has left the room";
    printf("%d %s %d\n", (int)it->second, leftMsg.c_str(), idRoom);
    this->m_mapRoomClients[idRoom].erase(username);

    // Succesfully removed user from room, notify everyone else in the room
    this->BroadcastToRoom(idRoom, username, leftMsg);

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
    for (std::pair<std::string, SOCKET> user : this->m_mapRoomClients[idRoom])
    {
        this->m_pTCP->SendRequest(user.second, myTcp::MSG_TYPE::CHAT_MESSAGE, idRoom, username, formatedMsg);
    }

    return;
}

void ChatServer::ExecuteIncommingMsgs()
{
    std::map<SOCKET, sPacketData> mapNewMsgs;
    this->m_pTCP->ReadNewMsgs(mapNewMsgs);

    for (std::pair<SOCKET, sPacketData> newMsg : mapNewMsgs)
    {
        SOCKET clientSocket = newMsg.first;
        sPacketData msgPacket = newMsg.second;

        if (msgPacket.header.packetSize == 0)
        {
            // Client disconnected, so we have to make sure he left the room first
            uint32 idRoomOut = 0;
            std::string usernameOut = "";
            this->GetRoomAndUsernameBySocket(clientSocket, idRoomOut, usernameOut);

            if (idRoomOut > 0)
            {
                // Client didn't leave the room, so we must remove him
                this->RemoveClientFromRoom(idRoomOut, usernameOut);
            }
            continue;
        } 
        
        if (msgPacket.header.msgType == myTcp::MSG_TYPE::ACTION)
        {
            // Find which action to take
            if (msgPacket.msg == "JOINROOM")
            {
                this->AddClientToRoom(msgPacket.header.idRoom, msgPacket.header.username, clientSocket);
                continue;
            }
            
            if (msgPacket.msg == "LEAVEROOM")
            {
                this->RemoveClientFromRoom(msgPacket.header.idRoom, msgPacket.header.username);
                continue;
            }

            printf("%s action doesn't exists\n", msgPacket.msg.c_str());

            continue;
        }

        if (msgPacket.header.msgType == myTcp::MSG_TYPE::CHAT_MESSAGE)
        {
            // Just a chat msg, so we send to everyone in the room
            this->BroadcastToRoom(msgPacket.header.idRoom, msgPacket.header.username, msgPacket.msg);

            continue;
        }
    }
}
