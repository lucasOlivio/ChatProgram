#include "ChatServer.h"

int main()
{
	ChatServer server = ChatServer();

	server.Initialize(NULL, DEFAULT_PORT);

	return 0;
}