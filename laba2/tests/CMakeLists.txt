find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(array_processor_tests test_main.cpp)
target_link_libraries(array_processor_tests GTest::GTest GTest::Main)
target_include_directories(array_processor_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME array_processor_tests COMMAND array_processor_tests)
