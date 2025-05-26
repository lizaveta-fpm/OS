cmake_minimum_required(VERSION 3.14)
project(laba3)

# Основной исполняемый файл
add_executable(laba3 main.cpp)

# Включение тестов
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
