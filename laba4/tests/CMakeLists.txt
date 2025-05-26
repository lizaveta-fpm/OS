# Поиск GoogleTest
find_package(GTest REQUIRED)
include(GoogleTest)

# Тесты для receiver
add_executable(test_receiver test_receiver.cpp)
target_link_libraries(test_receiver GTest::GTest GTest::Main)
target_include_directories(test_receiver PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_receiver COMMAND test_receiver)

# Тесты для sender
add_executable(test_sender test_sender.cpp)
target_link_libraries(test_sender GTest::GTest GTest::Main)
target_include_directories(test_sender PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME test_sender COMMAND test_sender)
