find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(marker_threads_tests test_main.cpp)
target_link_libraries(marker_threads_tests GTest::GTest GTest::Main)
target_include_directories(marker_threads_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_test(NAME marker_threads_tests COMMAND marker_threads_tests)
