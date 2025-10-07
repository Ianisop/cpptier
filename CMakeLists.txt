cmake_minimum_required(VERSION 3.20)
project(cpptier VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# Import fxtui
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

target_link_libraries(ctier_server
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  # PRIVATE ftxui::component  # only if using component module
)

target_link_libraries(ctier_client
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  # PRIVATE ftxui::component  # only if using component module
)

if(WIN32)
    target_link_libraries(ctier_server PRIVATE ws2_32)
    target_link_libraries(ctier_client PRIVATE ws2_32)
endif()

# Compiler warnings
#foreach(target ctier_server ctier_client)
#    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endforeach()
