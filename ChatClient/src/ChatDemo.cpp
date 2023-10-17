#include "ChatDemo.h"
#include <iostream>
#include <conio.h>

const char* DEFAULT_HOST = "127.0.0.1";
const uint32 MIN_ROOM_ID = 1;
const uint32 MAX_ROOM_ID = 255;

ChatDemo::ChatDemo()
{
	this->m_pChat = nullptr;
	this->m_roomMessages = {};
	this->m_isRunning = false;
}

ChatDemo::~ChatDemo()
{
}

bool ChatDemo::Initialize()
{
	this->m_pChat = new ChatClient();
	bool initialized = m_pChat->Initialize(DEFAULT_HOST, DEFAULT_PORT);

	if (!initialized)
	{
		printf("Error connecting to server!\n\n");
		system("pause");
		return false;
	}
	return true;
}

void ChatDemo::Destroy()
{
	this->m_roomMessages.clear();
	this->m_pChat->Destroy();
	delete this->m_pChat;

	return;
}

void ChatDemo::RunChat()
{
	this->m_isRunning = true;
	while (this->m_isRunning)
	{
		if (this->m_pChat->IsInRoom())
		{
			InRoomScene();
		}
		else
		{
			OutRoomScene();
		}
	}

	this->Destroy();

	return;
}

void ChatDemo::OutRoomScene()
{
	uint32 idRoom;
	std::string username = "";
	std::string errorMsg = "";

	printf("Join room id (Between %d and %d, or 0 to exit): ", MIN_ROOM_ID, MAX_ROOM_ID);
	std::cin >> idRoom;
	if (idRoom == 0)
	{
		// Exiting program;
		this->m_isRunning = false;
		return;
	}
	if (idRoom < MIN_ROOM_ID || idRoom > MAX_ROOM_ID)
	{
		printf("Room id not in range! Try again\n");
		return;
	}

	printf("Enter username: ");
	std::cin >> username;

	// Try to enter room with this username
	if (!this->m_pChat->JoinRoom(idRoom, username, errorMsg))
	{
		// couldn't join
		printf("Could not join room: %s\n\n", errorMsg.c_str());
		return;
	}

	// Entered room, so clear msgs vector
	this->m_roomMessages.clear();

	return;
}

void ChatDemo::PrintAllMsgs()
{
	for (std::string msg : this->m_roomMessages)
	{
		printf("%s\n", msg.c_str());
	}
	return;
}

void ChatDemo::RedrawInRoomScene()
{
	system("cls");
	this->PrintAllMsgs();

	printf("\n\n");

	printf("Send message (':q' to leave room, ESC to exit the program): \n");
	printf("%s", this->m_myMsg.c_str());

	return;
}

void ChatDemo::InRoomScene()
{
	std::string newMsg = this->m_pChat->ReceiveRoomMsg();

	if (newMsg != "")
	{
		// There is new msg, add to cache and print
		this->m_roomMessages.push_back(newMsg);
		this->RedrawInRoomScene();
	}

	if (_kbhit())
	{
		int key = _getch();
		if (key == 27) /*ESC*/
		{ 
			// Leave program
			this->m_isRunning = false;
			return;
		}
		if (key == 13) /*ENTER*/
		{ 
			if (this->m_myMsg == ":q")
			{
				// Leave room and clear msg cache and scene
				this->m_pChat->LeaveRoom();
				this->m_roomMessages.clear();
				system("cls");
			} 
			else
			{
				// New message sent
				this->m_pChat->SendChatMessage(this->m_myMsg);
			}

			this->m_myMsg = "";
			return;
		}

		if (key == 8) /*BACKSPACE*/
		{
			if (!this->m_myMsg.empty()) {
				this->m_myMsg.pop_back(); // Remove last char
			}
		}
		else
		{
			// Anything else we add to the msg been typed by the user
			this->m_myMsg += static_cast<char>(key);
		}

		this->RedrawInRoomScene();
	}

	return;
}
