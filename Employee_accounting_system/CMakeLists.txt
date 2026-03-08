cmake_minimum_required(VERSION 3.28)
project(lab3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)


add_executable(lab3 src/main.cpp
        src/Leader.cpp
        src/Worker.cpp
        src/Data.cpp
        src/MainData.cpp
        src/Controller.cpp
        src/View.cpp
        src/input.cpp
)
target_include_directories(lab3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
add_executable(my_test
        tests/t.cpp
        src/Leader.cpp
        src/Worker.cpp
        src/Data.cpp
        src/MainData.cpp
        src/Controller.cpp
        src/View.cpp
        src/input.cpp
)
target_include_directories(my_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_link_libraries(my_test PRIVATE
        Catch2::Catch2WithMain
        # instruct_detail_lib
)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
endif()
add_test(NAME MyTests COMMAND my_test)
