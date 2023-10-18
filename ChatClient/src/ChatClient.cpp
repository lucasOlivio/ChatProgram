#include "ChatClient.h"

ChatClient::ChatClient()
{
	this->m_idRoom = 0;
    this->m_username = "";
	this->m_pTCP = nullptr;
    this->m_isInitialized = false;
}

ChatClient::~ChatClient()
{
}

bool ChatClient::Initialize(const char* host, const char* port)
{
    if (this->m_isInitialized)
    {
        // Already initialized
        return true;
    }

    // Initialize WSA and socket
    this->m_pTCP = new TCPClient();
    bool TCPInitialized = this->m_pTCP->Initialize(host, port);
    if (!TCPInitialized)
    {
        printf("Error initializing TCP!");
        return false;
    }

    this->m_isInitialized = true;
    return true;
}

void ChatClient::Destroy()
{
    if (this->m_idRoom)
    {
        // If its in a room, we must exit the room first to clear the username
        this->LeaveRoom();
    }
    this->m_pTCP->Destroy();
    delete this->m_pTCP;
    this->m_isInitialized = false;
    return;
}

bool ChatClient::IsInRoom()
{
    if (this->m_idRoom > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ChatClient::SendChatMessage(const std::string& msg)
{
    this->m_pTCP->SendRequest(this->m_pTCP->GetSocket(), myTcp::MSG_TYPE::CHAT_MESSAGE, 
                                this->m_idRoom, this->m_username, msg);
    return;
}

bool ChatClient::JoinRoom(uint32 idRoom, std::string username, std::string& errorMsgOut)
{
    // Send the request to server with the choosen username
    this->m_pTCP->SendRequest(this->m_pTCP->GetSocket(), myTcp::MSG_TYPE::ACTION, idRoom, username, "JOINROOM");

    // Wait for the server's response if the user was able to join the room
    sPacketData packetOut = sPacketData();
    if (this->m_pTCP->CheckMsgFromServer(3))
    {
        this->m_pTCP->ReceiveRequest(this->m_pTCP->GetSocket(), packetOut);
    }
    else
    {
        // No response from server
        packetOut.header.msgType = myTcp::MSG_TYPE::RESPONSE;
        packetOut.msg = "timeout";
    }

    if (packetOut.header.msgType == myTcp::MSG_TYPE::RESPONSE && packetOut.msg != "ok")
    {
        // Error trying to put user in new room
        errorMsgOut = packetOut.msg;
        return false;
    }

    // User inserted in new room, we can update our current room
    this->m_idRoom = idRoom;
    this->m_username = username;
    errorMsgOut = "";

    return true;
}

void ChatClient::LeaveRoom()
{
    // Send the request to server with the choosen username
    this->m_pTCP->SendRequest(this->m_pTCP->GetSocket(), myTcp::MSG_TYPE::ACTION, this->m_idRoom, 
                                    this->m_username, "LEAVEROOM");
    this->m_idRoom = 0;
    return;
}

std::string ChatClient::ReceiveRoomMsg()
{
    bool isNewMsgAvailable = this->m_pTCP->CheckMsgFromServer();
    if (!isNewMsgAvailable)
    {
        // No new messages
        return "";
    }

    sPacketData newMsg = sPacketData();
    this->m_pTCP->ReceiveRequest(this->m_pTCP->GetSocket(), newMsg);

    if (newMsg.header.packetSize == 0)
    {
        // Server disconnected, so should we
        printf("Server disconnected!\n");
        this->m_idRoom = 0;
        this->Destroy();
        return "";
    }

    return newMsg.msg;
}
