#include "ChatServer.h"
#include <conio.h>

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

	printf("Press ESC to close\n");
	while (true)
	{
		server.ExecuteIncommingMsgs();

		if (_kbhit())
		{
			int key = _getch();
			if (key == 27) /*ESC*/
			{
				// Leave program
				break;
			}
		}
	}

	server.Destroy();

	return 0;
}