#include <gtest/gtest.h>
#include <windows.h>
#include <vector>
#include <atomic>
#include "main.cpp"  // Подключаем исходный файл для тестирования

// Мок-класс для тестирования
class MarkerThreadTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Инициализация тестовых данных
        arr_size = 10;
        arr = new int[arr_size]();
        num = 3;
        
        // Инициализация событий
        stopEvents = new HANDLE[num];
        continueEvents = new HANDLE[num];
        isWorking = new bool[num];
        
        InitializeCriticalSection(&cs);
        
        for (int i = 0; i < num; i++) {
            stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
            continueEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
            isWorking[i] = true;
        }
    }

    void TearDown() override {
        delete[] arr;
        delete[] stopEvents;
        delete[] continueEvents;
        delete[] isWorking;
        DeleteCriticalSection(&cs);
    }
};

// Тест функции marker
TEST_F(MarkerThreadTest, MarkerThreadBasicOperation) {
    // Подготовка тестовых данных
    int thread_num = 0;
    isWorking[thread_num] = true;
    
    // Запуск потока
    HANDLE hThread = CreateThread(NULL, 0, marker, (void*)thread_num, 0, NULL);
    
    // Даем потоку время на работу
    Sleep(100);
    
    // Останавливаем поток
    EnterCriticalSection(&cs);
    isWorking[thread_num] = false;
    SetEvent(continueEvents[thread_num]);
    LeaveCriticalSection(&cs);
    
    // Ожидаем завершения
    WaitForSingleObject(hThread, INFINITE);
    
    // Проверяем результат
    bool hasMarkedElements = false;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] == thread_num + 1) {
            FAIL() << "Thread didn't clean up marked elements";
        }
        if (arr[i] != 0) {
            hasMarkedElements = true;
        }
    }
    
    EXPECT_TRUE(hasMarkedElements) << "Thread didn't mark any elements";
    CloseHandle(hThread);
}

// Тест критической секции
TEST_F(MarkerThreadTest, CriticalSectionProtection) {
    // Создаем два потока
    HANDLE threads[2];
    std::atomic<int> counter(0);
    
    // Тестовая функция
    auto test_func = [&](LPVOID) {
        EnterCriticalSection(&cs);
        counter++;
        LeaveCriticalSection(&cs);
        return 0;
    };
    
    // Запускаем потоки
    for (int i = 0; i < 2; i++) {
        threads[i] = CreateThread(NULL, 0, test_func, NULL, 0, NULL);
    }
    
    // Ожидаем завершения
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);
    
    // Проверяем, что счетчик увеличился правильно
    EXPECT_EQ(counter, 2);
    
    for (int i = 0; i < 2; i++) {
        CloseHandle(threads[i]);
    }
}

// Тест функции resetEvents
TEST_F(MarkerThreadTest, ResetEventsFunction) {
    // Устанавливаем все события
    for (int i = 0; i < num; i++) {
        SetEvent(stopEvents[i]);
    }
    
    // Сбрасываем события
    resetEvents(stopEvents);
    
    // Проверяем, что все события сброшены
    for (int i = 0; i < num; i++) {
        EXPECT_EQ(WaitForSingleObject(stopEvents[i], 0), WAIT_TIMEOUT);
    }
}

// Тест функции showArray
TEST_F(MarkerThreadTest, ShowArrayFunction) {
    // Подготавливаем тестовый массив
    for (int i = 0; i < arr_size; i++) {
        arr[i] = i + 1;
    }
    
    // Перенаправляем вывод для проверки
    testing::internal::CaptureStdout();
    showArray();
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверяем вывод
    for (int i = 0; i < arr_size; i++) {
        std::string expected = std::to_string(i + 1) + ": " + std::to_string(i + 1) + "\n";
        EXPECT_NE(output.find(expected), std::string::npos);
    }
}

// Тест основного потока выполнения
TEST_F(MarkerThreadTest, MainThreadWorkflow) {
    // Упрощенная версия main() для тестирования
    HANDLE testThread = CreateThread(NULL, 0, marker, (void*)0, 0, NULL);
    
    // Даем потоку время на работу
    Sleep(100);
    
    // Имитируем остановку потока
    isWorking[0] = false;
    SetEvent(continueEvents[0]);
    
    // Ожидаем завершения
    WaitForSingleObject(testThread, INFINITE);
    
    // Проверяем состояние массива
    bool allCleared = true;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] != 0) {
            allCleared = false;
            break;
        }
    }
    
    EXPECT_TRUE(allCleared) << "Thread didn't clean up properly";
    CloseHandle(testThread);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
