#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include "main.cpp"  // Подключаем исходный файл для тестирования

// Тестовый класс
class ThreadArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Инициализация тестовых данных
        testNumbers = {5, 2, 8, 1, 9, 3};
        numbers = testNumbers;
    }

    vector<int> testNumbers;
};

// Тест функции MinMaxThread
TEST_F(ThreadArrayTest, MinMaxCalculation) {
    MinMaxThread(nullptr);
    
    EXPECT_EQ(min_value, *min_element(testNumbers.begin(), testNumbers.end()));
    EXPECT_EQ(max_value, *max_element(testNumbers.begin(), testNumbers.end()));
}

// Тест функции AverageThread
TEST_F(ThreadArrayTest, AverageCalculation) {
    AverageThread(nullptr);
    
    double expected = accumulate(testNumbers.begin(), testNumbers.end(), 0.0) / testNumbers.size();
    EXPECT_DOUBLE_EQ(average_value, expected);
}

// Тест замены min/max на average
TEST_F(ThreadArrayTest, ReplaceMinMaxWithAverage) {
    // Вычисляем ожидаемые значения
    double avg = accumulate(testNumbers.begin(), testNumbers.end(), 0.0) / testNumbers.size();
    int min_val = *min_element(testNumbers.begin(), testNumbers.end());
    int max_val = *max_element(testNumbers.begin(), testNumbers.end());
    
    // Вызываем функции потоков
    MinMaxThread(nullptr);
    AverageThread(nullptr);
    
    // Заменяем значения
    for (int& num : numbers) {
        if (num == min_value || num == max_value) {
            num = static_cast<int>(average_value);
        }
    }
    
    // Проверяем замену
    for (int num : numbers) {
        if (num == min_val || num == max_val) {
            FAIL() << "Min/Max values were not replaced";
        }
    }
    
    // Проверяем, что все замененные значения равны среднему
    for (int num : numbers) {
        if (num == static_cast<int>(avg)) {
            continue;
        }
        EXPECT_NE(num, min_val);
        EXPECT_NE(num, max_val);
    }
}

// Мок-тест для проверки создания потоков
TEST_F(ThreadArrayTest, ThreadCreation) {
    // Здесь можно добавить моки для Windows API, если используется библиотека для мокинга
    // В этом примере просто проверяем, что функции потоков работают
    MinMaxThread(nullptr);
    AverageThread(nullptr);
    
    EXPECT_NE(min_value, max_value);  // Простая проверка
    EXPECT_GT(average_value, min_value);
    EXPECT_LT(average_value, max_value);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
