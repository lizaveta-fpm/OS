#include <gtest/gtest.h>
#include "Message.h"
#include <fstream>

class ReceiverTest : public ::testing::Test {
protected:
    void SetUp() override {
        filename = "test.bin";
        messageCount = 10;
    }

    void TearDown() override {
        remove(filename.c_str());
    }

    std::string filename;
    int messageCount;
};

TEST_F(ReceiverTest, CreatesBinaryFile) {
    // Тест создания бинарного файла
    std::fstream file(filename, std::ios::out | std::ios::binary);
    ASSERT_TRUE(file.is_open());
    
    Message msg;
    msg.isEmpty = true;
    file.write((char*)&msg, sizeof(Message));
    file.close();
    
    std::ifstream inFile(filename, std::ios::binary);
    Message readMsg;
    inFile.read((char*)&readMsg, sizeof(Message));
    ASSERT_TRUE(readMsg.isEmpty);
}
