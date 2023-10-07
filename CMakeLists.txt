cmake_minimum_required(VERSION 3.26)
project(column_store)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()


add_executable(column_store main.cpp table.cpp)

add_executable(column_test column_test.cpp)
target_link_libraries(column_test GTest::gtest_main)

add_executable(table_test table_test.cpp table.cpp)
target_link_libraries(table_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(column_test)


