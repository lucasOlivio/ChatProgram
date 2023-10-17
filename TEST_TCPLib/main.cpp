#include "common.h"
#include "PacketTypes.h"
#include "Buffer.h"
#include <gtest/gtest.h>

struct BufferTestFixture : testing::Test {
public:
	Buffer* pBuffer;

	BufferTestFixture() : pBuffer(nullptr) {}

	virtual ~BufferTestFixture() {}

	void SetUp() {
		pBuffer = new Buffer(512);
	}

	virtual void TearDown() {
		delete pBuffer;
	}
};

// GOOD CASES
// ------------------------------------------------------------------

// An int32 after write and read should return same thing
TEST_F(BufferTestFixture, GOODCASE_WriteAndReadEqualINT32) {
	uint32 testInt = 255;

	pBuffer->WriteUInt32LE(testInt);
	uint32 result = pBuffer->ReadUInt32LE();

	EXPECT_EQ(testInt, result);
}

// An int16 after write and read should return same thing
TEST_F(BufferTestFixture, GOODCASE_WriteAndReadEqualINT16) {
	uint16 testInt = 55;

	pBuffer->WriteUInt16LE(testInt);
	uint16 result = pBuffer->ReadUInt16LE();

	EXPECT_EQ(testInt, result);
}

// An string after write and read should return same thing
TEST_F(BufferTestFixture, GOODCASE_WriteAndReadEqualStr) {
	std::string testStr = "testing";

	pBuffer->WriteString(testStr);
	std::string result = pBuffer->ReadString(testStr.length());

	EXPECT_EQ(testStr, result);
}

// Multiple data serialized in buffer should return the same
TEST_F(BufferTestFixture, GOODCASE_WriteAndReadEqualMultipleData) {
	uint32 headerId = 14;
	std::string msgTest = "testing";

	pBuffer->WriteUInt32LE(headerId);
	pBuffer->WriteString(msgTest);

	uint32 outId = pBuffer->ReadUInt32LE();
	std::string outMsg = pBuffer->ReadString(msgTest.length());

	EXPECT_EQ(headerId, outId);
	EXPECT_EQ(msgTest, outMsg);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}