#pragma once

#include "common.h"

struct sPacketHeader
{
    uint32 packetSize;
    uint32 msgType;
    uint32 idRoom;
};

struct sPacketData
{
    sPacketHeader header;
    uint32 msgLength;
    std::string msg;
};