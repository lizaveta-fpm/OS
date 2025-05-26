cmake_minimum_required(VERSION 3.14)
project(laba4)

# Основные исполняемые файлы
add_executable(receiver src/receiver.cpp)
add_executable(sender src/sender.cpp)

# Включить тесты
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
