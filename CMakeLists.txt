cmake_minimum_required(VERSION 3.20)
project(cpptier VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Build executables
add_executable(ctier_server
    src/c_tier_server.cpp
    src/server/server.cpp
    src/socket/websock.cpp
)
add_executable(ctier_client
    src/c_tier_client.cpp
    src/client/client.cpp
    src/socket/websock.cpp
)

# Include paths
target_include_directories(ctier_server PRIVATE src/socket)
target_include_directories(ctier_client PRIVATE src/socket)

# Compiler warnings
#foreach(target ctier_server ctier_client)
#    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endforeach()
