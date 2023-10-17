#pragma once

#include "common.h"

// TODO: Improve it so that we can pass parameters without changing the whole header structure
struct sPacketHeader
{
    uint32 packetSize;
    uint32 msgType;
    uint32 idRoom;
    uint32 usernameLength;
    std::string username;
};

struct sPacketData
{
    sPacketHeader header;
    uint32 msgLength;
    std::string msg;
};