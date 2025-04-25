cmake_minimum_required(VERSION 3.10)
project(my-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

add_executable(my-project
    src/main.cpp
    src/archiver.cpp
)

target_link_libraries(my-project ${ZLIB_LIBRARIES})

if (WIN32)
    message(STATUS "Compiling on Windows")
    if (MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()
else()
    message(STATUS "Compiling on Unix-like system")
endif()
