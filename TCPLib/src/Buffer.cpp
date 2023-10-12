#include "Buffer.h"

Buffer::Buffer(uint size)
{
    this->vecBufferData.resize(size);
    this->m_writeIndex = 0;
    this->m_readIndex = 0;
}

Buffer::~Buffer()
{
}

void Buffer::WriteUInt32LE(uint32 index, uint32 value)
{
}

void Buffer::WriteUInt32LE(uint32 value)
{
}

void Buffer::WriteUInt16LE(uint32 index, uint16 value)
{
}

void Buffer::WriteUInt16LE(uint16 value)
{
}

void Buffer::WriteString(uint32 index, const std::string& value)
{
}

void Buffer::WriteString(const std::string& value)
{
}

uint32 Buffer::ReadUInt32LE(uint32 index)
{
    return uint32();
}

uint32 Buffer::ReadUInt32LE()
{
    return uint32();
}

uint16 Buffer::ReadUInt16LE(uint32 index)
{
    return uint16();
}

uint16 Buffer::ReadUInt16LE()
{
    return uint16();
}

std::string Buffer::ReadString(uint32 index)
{
    return std::string();
}

std::string Buffer::ReadString()
{
    return std::string();
}
