cmake_minimum_required(VERSION 3.20)
project(cpptier VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#Gather sources
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS src/server/*.cpp src/socket/*.cpp)
file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS src/client/*.cpp src/socket/*.cpp)

# Build executables
add_executable(ctier_server src/c_tier_server.cpp ${SERVER_SOURCES})
add_executable(ctier_client src/c_tier_client.cpp ${CLIENT_SOURCES})

# Include paths
target_include_directories(ctier_server PRIVATE src/socket)
target_include_directories(ctier_client PRIVATE src/socket)

# Compiler warnings
#foreach(target ctier_server ctier_client)
#    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endforeach()
