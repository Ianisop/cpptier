cmake_minimum_required(VERSION 3.20)
project(cpptier VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# Import ftxui
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v6.1.9
)

# Avoid re-declaring/populating multiple times
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR})
endif()

FetchContent_Declare(
    sqlitecpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
    GIT_TAG        3.3.1
)

FetchContent_MakeAvailable(sqlitecpp)

# Set SQLiteCpp options to avoid build issues
set(SQLITECPP_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SQLITECPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# Include current source directory for headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Build executables
add_executable(ctier_server
    src/c_tier_server.cpp
    src/server/server.cpp
    src/socket/websock.cpp
    src/database/database.cpp
)

add_executable(ctier_client
    src/c_tier_client.cpp
    src/client/client.cpp
    src/socket/websock.cpp
    src/database/database.cpp
)

# Include paths - FIXED: Include the base source directory
target_include_directories(ctier_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/socket
    ${CMAKE_CURRENT_SOURCE_DIR}/src/database
)

target_include_directories(ctier_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/socket
    ${CMAKE_CURRENT_SOURCE_DIR}/src/database
)

find_package(OpenSSL REQUIRED)

# Link libraries - FIXED: Use proper target names
target_link_libraries(ctier_server PRIVATE
    ftxui::component
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLiteCpp
)

target_link_libraries(ctier_client PRIVATE
    ftxui::component
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLiteCpp
)

if(WIN32)
    target_link_libraries(ctier_server PRIVATE ws2_32)
    target_link_libraries(ctier_client PRIVATE ws2_32)
endif()
