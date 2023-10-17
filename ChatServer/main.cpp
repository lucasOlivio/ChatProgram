#include "ChatServer.h"

int main()
{
	ChatServer server = ChatServer();

	bool initialized = server.Initialize(NULL, DEFAULT_PORT);

	if (!initialized)
	{
		printf("Error initializing server!");
		system("pause");
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		server.ExecuteIncommingMsgs();
	}

	server.Destroy();

	return 0;
}