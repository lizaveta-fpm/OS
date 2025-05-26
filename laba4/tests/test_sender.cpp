#include <gtest/gtest.h>
#include "Message.h"
#include <fstream>

class SenderTest : public ::testing::Test {
protected:
    void SetUp() override {
        filename = "test.bin";
        messageCount = 10;
        
        // Создаем тестовый файл
        std::fstream file(filename, std::ios::out | std::ios::binary);
        Message msg;
        msg.isEmpty = true;
        for (int i = 0; i < messageCount; ++i) {
            file.write((char*)&msg, sizeof(Message));
        }
        file.close();
    }

    void TearDown() override {
        remove(filename.c_str());
    }

    std::string filename;
    int messageCount;
};

TEST_F(SenderTest, WritesMessageToFile) {
    // Тест записи сообщения
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
    ASSERT_TRUE(file.is_open());
    
    Message msg;
    strcpy_s(msg.text, "Test message");
    msg.isEmpty = false;
    
    file.seekp(0);
    file.write((char*)&msg, sizeof(Message));
    file.close();
    
    std::ifstream inFile(filename, std::ios::binary);
    Message readMsg;
    inFile.read((char*)&readMsg, sizeof(Message));
    ASSERT_STREQ(readMsg.text, "Test message");
    ASSERT_FALSE(readMsg.isEmpty);
}
