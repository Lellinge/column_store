cmake_minimum_required(VERSION 3.22)
project(column_store)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

IF (WIN32)
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
ELSE ()

ENDIF ()

FetchContent_MakeAvailable(googletest)

# Fetch the project and make it available for use.
include(FetchContent)
FetchContent_Declare(
        libcoro
        GIT_REPOSITORY https://github.com/jbaldwin/libcoro.git
        GIT_TAG        v0.9
)
FetchContent_MakeAvailable(libcoro)


enable_testing()

add_subdirectory(pybind11)



add_executable(column_store main.cpp table.cpp
        column.cpp)

add_executable(column_test column_test.cpp
        column.cpp)
target_link_libraries(column_test GTest::gtest_main)

add_executable(table_test table_test.cpp table.cpp
        column.cpp)
target_link_libraries(table_test GTest::gtest_main)

add_executable(operators_test operators_test.cpp operators.cpp table.cpp column.cpp)
target_link_libraries(operators_test GTest::gtest_main)
target_link_libraries(operators_test libcoro)

include(GoogleTest)
gtest_discover_tests(column_test)


pybind11_add_module(pystore pymodule.cpp table.h table.cpp column.cpp column.h)
