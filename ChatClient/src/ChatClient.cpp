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
    this->m_pTCP->Destroy();
    delete this->m_pTCP;
    this->m_isInitialized = false;
    return;
}

void ChatClient::SendChatMessage(const string& msg)
{
    this->m_pTCP->SendRequest(this->m_pTCP->GetSocket(), MSG_TYPE::CHAT_MESSAGE, this->m_idRoom, msg);
    return;
}

bool ChatClient::JoinRoom(uint32 idRoom, string username, string& errorMsgOut)
{
    // Send the request to server with the choosen username
    this->m_pTCP->SendRequest(this->m_pTCP->GetSocket(), MSG_TYPE::ACTION, idRoom, "JOINROOM/"+username);

    // Wait for the server's response if the user was able to join the room
    MSG_TYPE msgTypeOut;
    std::string response;
    this->m_pTCP->ReceiveRequest(this->m_pTCP->GetSocket(), msgTypeOut, 0, response);

    if (msgTypeOut == MSG_TYPE::RESPONSE && response != "ok")
    {
        // Error trying to put user in new room
        errorMsgOut = response;
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
    this->m_pTCP->SendRequest(this->m_pTCP->GetSocket(), MSG_TYPE::ACTION, this->m_idRoom, "LEAVEROOM/" + this->m_username);
    return;
}
