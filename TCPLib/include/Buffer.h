#pragma once

#include "common.h"

class Buffer {
private:
    // The index to write the next byte of data in the buffer
    uint32 m_writeIndex;

    // The index to read the next byte of data from the buffer
    uint32 m_readIndex;

public:
    // this stores all of the data within the buffer
    std::vector<uint8_t> vecBufferData;

    // ctors & dtors
    Buffer(uint size);
    ~Buffer();
    
    // Resizes the vector if index bigger that actual size
    void CheckSize(uint32 index, uint32 valueSize);

    // Serializes data to the buffer vector
    void WriteUInt32LE(uint32 index, uint32 value);
    void WriteUInt32LE(uint32 value);
    void WriteUInt16LE(uint32 index, uint16 value);
    void WriteUInt16LE(uint16 value);
    void WriteString(uint32 index, const std::string& value);
    void WriteString(const std::string& value);

    // Deserializes data from the buffer vector
    uint32 ReadUInt32LE(uint32 index);
    uint32 ReadUInt32LE();
    uint16 ReadUInt16LE(uint32 index);
    uint16 ReadUInt16LE();
    std::string ReadString(uint32 index, uint32 strLength);
    std::string ReadString(uint32 strLength);
};