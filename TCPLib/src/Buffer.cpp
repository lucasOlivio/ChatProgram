#include "Buffer.h"

Buffer::Buffer(uint size)
{
    this->vecBufferData.reserve(size);
    this->m_writeIndex = 0;
    this->m_readIndex = 0;
}

Buffer::~Buffer()
{
    this->vecBufferData.clear();
}

void Buffer::CheckSize(uint32 index, uint32 valueSize)
{
    // Get total that will be written
    uint32 endIndex = index + valueSize;

    if (endIndex < this->vecBufferData.size()) {
        // Index smaller, no changes needed
        return;
    }

    // Allocates enough space for all the value
    this->vecBufferData.resize(endIndex + 1, 0);
    
    return;
}

void Buffer::WriteUInt32LE(uint32 index, uint32 value)
{
    this->CheckSize(index, sizeof(value));

    // Now we go 8 by 8 bits setting the value in reverse
    for (unsigned int i = index; i < sizeof(value); i++) {
        this->vecBufferData[i] = value >> (i * 8);
    }
}

void Buffer::WriteUInt32LE(uint32 value)
{
    this->CheckSize(this->m_writeIndex, sizeof(value));

    // Now we go 8 by 8 bits setting the value in reverse
    unsigned int endIndex = this->m_writeIndex + sizeof(value);
    for (unsigned int i = this->m_writeIndex; i < endIndex; i++) {
        this->vecBufferData[i] = value >> (i * 8);
    }

    this->m_writeIndex += sizeof(value);
}

void Buffer::WriteUInt16LE(uint32 index, uint16 value)
{
    this->CheckSize(index, sizeof(value));

    // Now we go 8 by 8 bits setting the value in reverse
    for (unsigned int i = index; i < sizeof(value); i++) {
        this->vecBufferData[i] = value >> (i * 8);
    }
}

void Buffer::WriteUInt16LE(uint16 value)
{
    this->CheckSize(this->m_writeIndex, sizeof(value));

    // Now we go 8 by 8 bits setting the value in reverse
    unsigned int endIndex = this->m_writeIndex + sizeof(value);
    for (unsigned int i = this->m_writeIndex; i < endIndex; i++) {
        this->vecBufferData[i] = value >> (i * 8);
    }

    this->m_writeIndex += sizeof(value);
}

void Buffer::WriteString(uint32 index, const std::string& value)
{
    this->CheckSize(index, value.length());

    int indexEnd = index + value.length();
    // Set char by char from the string to the buffer data
    for (int i = index; i < indexEnd; i++)
    {
        this->vecBufferData[index++] = value[i];
    }
}

void Buffer::WriteString(const std::string& value)
{
    this->CheckSize(this->m_writeIndex, value.length());

    int strLength = value.length();
    // Set char by char from the string to the buffer data
    for (int i = 0; i < strLength; i++)
    {
        this->vecBufferData[this->m_writeIndex++] = value[i];
    }
}

uint32 Buffer::ReadUInt32LE(uint32 index)
{
    if (index >= this->vecBufferData.size()) {
        // Index already at the end
        return 0;
    }
   
    uint32 res = this->vecBufferData[index];
    this->m_readIndex = index;
    unsigned int endIndex = this->m_readIndex + sizeof(res);

    // Reverse back 8 by 8 bits from buffer data
    for (unsigned int i = this->m_readIndex; i < endIndex; i++) {
        res |= this->vecBufferData[i] << (i * 8);
    }

    this->m_readIndex += sizeof(res);

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
        res |= this->vecBufferData[i] << (i * 8);
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
    uint16 res = this->vecBufferData[index];
    for (unsigned int i = index; i < sizeof(this->vecBufferData); i++) {
        res |= this->vecBufferData[i] << (i * 8);
    }

    return res;
}

uint16 Buffer::ReadUInt16LE()
{
    if (this->m_readIndex >= this->vecBufferData.size()) {
        // Index already at the end
        return 0;
    }

    uint16 res = this->vecBufferData[this->m_readIndex];
    unsigned int endIndex = this->m_readIndex + sizeof(res);

    // Reverse back 8 by 8 bits from buffer data
    for (unsigned int i = this->m_readIndex; i < endIndex; i++) {
        res |= this->vecBufferData[i] << (i * 8);
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
