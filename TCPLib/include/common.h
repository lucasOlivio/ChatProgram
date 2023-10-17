#pragma once

#include <vector>
#include <string>

typedef unsigned int uint;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

namespace myTcp
{
	#define DEFAULT_PORT "8811"
	enum MSG_TYPE
	{
		ACTION,
		CHAT_MESSAGE,
		RESPONSE
	};
}