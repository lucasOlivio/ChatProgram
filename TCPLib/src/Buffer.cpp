#include "Buffer.h"

Buffer::Buffer(uint size)
{
    this->vecBufferData.resize(size, 0);
    this->m_writeIndex = 0;
    this->m_readIndex = 0;
}

Buffer::~Buffer()
{
    this->vecBufferData.clear();
}

void Buffer::WriteUInt32LE(uint32 index, uint32 value)
{
    if (index >= this->vecBufferData.size()) {
        // Index already at the end
        return;
    }

    // Now we go 8 by 8 bits setting the value in reverse
    for (unsigned int i = index; i < sizeof(value); i++) {
        this->vecBufferData[index + i] = value >> (i * 8);
    }
}

void Buffer::WriteUInt32LE(uint32 value)
{
    if (this->m_writeIndex >= this->vecBufferData.size()) {
        // Write index already at the end
        return;
    }

    // Now we go 8 by 8 bits setting the value in reverse
    unsigned int endIndex = this->m_writeIndex + sizeof(value);
    for (unsigned int i = this->m_writeIndex; i < endIndex; i++) {
        this->vecBufferData[this->m_writeIndex + i] = value >> (i * 8);
    }

    this->m_writeIndex += sizeof(value);
}

void Buffer::WriteUInt16LE(uint32 index, uint16 value)
{
    if (index >= this->vecBufferData.size()) {
        // Index already at the end
        return;
    }

    // Now we go 8 by 8 bits setting the value in reverse
    for (unsigned int i = index; i < sizeof(value); i++) {
        this->vecBufferData[index + i] = value >> (i * 8);
    }
}

void Buffer::WriteUInt16LE(uint16 value)
{
    if (this->m_writeIndex >= this->vecBufferData.size()) {
        // Write index already at the end
        return;
    }

    // Now we go 8 by 8 bits setting the value in reverse
    unsigned int endIndex = this->m_writeIndex + sizeof(value);
    for (unsigned int i = this->m_writeIndex; i < endIndex; i++) {
        this->vecBufferData[this->m_writeIndex + i] = value >> (i * 8);
    }

    this->m_writeIndex += sizeof(value);
}

void Buffer::WriteString(uint32 index, const std::string& value)
{
    if (index >= this->vecBufferData.size()) {
        // Index already at the end
        return;
    }

    int indexEnd = index + value.length();
    // Set char by char from the string to the buffer data
    for (int i = index; i < indexEnd; i++)
    {
        this->vecBufferData[index++] = value[i];
    }
}

void Buffer::WriteString(const std::string& value)
{
    if (this->m_writeIndex >= this->vecBufferData.size()) {
        // Write index already at the end
        return;
    }

    int strLength = value.length();
    // Set char by char from the string to the buffer data
    for (int i = 0; i < strLength; i++)
    {
        this->vecBufferData[this->m_writeIndex++] = value[i];
    }

    this->m_writeIndex += strLength;
}

uint32 Buffer::ReadUInt32LE(uint32 index)
{
    if (index >= this->vecBufferData.size()) {
        // Index already at the end
        return 0;
    }
   
    // Reverse back 8 by 8 bits from buffer data
    uint32 res = this->vecBufferData[index];

    for (unsigned int i = index; i < sizeof(this->vecBufferData); i++) {
        res |= this->vecBufferData[index + i] << (i * 8);
    }

    return res;
}

uint32 Buffer::ReadUInt32LE()
{
    if (this->m_readIndex >= this->vecBufferData.size()) {
        // Index already at the end
        return 0;
    }

    uint32 res = this->vecBufferData[this->m_readIndex];
    unsigned int endIndex = this->m_readIndex + sizeof(res);

    // Reverse back 8 by 8 bits from buffer data
    for (unsigned int i = this->m_readIndex; i < endIndex; i++) {
        res |= this->vecBufferData[this->m_readIndex + i] << (i * 8);
    }

    this->m_readIndex += sizeof(res);

    return res;
}

uint16 Buffer::ReadUInt16LE(uint32 index)
{
    if (index >= this->vecBufferData.size()) {
        // Index already at the end
        return 0;
    }

    // Reverse back 8 by 8 bits from buffer data
    uint32 res = this->vecBufferData[index];
    for (unsigned int i = index; i < sizeof(this->vecBufferData); i++) {
        res |= this->vecBufferData[index + i] << (i * 8);
    }

    return res;
}

uint16 Buffer::ReadUInt16LE()
{
    if (this->m_readIndex >= this->vecBufferData.size()) {
        // Index already at the end
        return 0;
    }

    uint32 res = this->vecBufferData[this->m_readIndex];
    unsigned int endIndex = this->m_readIndex + sizeof(res);

    // Reverse back 8 by 8 bits from buffer data
    for (unsigned int i = this->m_readIndex; i < endIndex; i++) {
        res |= this->vecBufferData[this->m_readIndex + i] << (i * 8);
    }

    this->m_readIndex += sizeof(res);

    return res;
}

std::string Buffer::ReadString(uint32 index, uint32 strLength)
{
    if (index >= this->vecBufferData.size()) {
        // Index already at the end
        return "";
    }

    std::string str;
    for (int i = 0; i < strLength; i++)
    {
        str.push_back(this->vecBufferData[index++]);
    }
    return str;
}

std::string Buffer::ReadString(uint32 strLength)
{
    if (this->m_readIndex >= this->vecBufferData.size()) {
        // Index already at the end
        return "";
    }

    std::string str;
    for (int i = 0; i < strLength; i++)
    {
        str.push_back(this->vecBufferData[this->m_readIndex++]);
    }
    return str;
}
