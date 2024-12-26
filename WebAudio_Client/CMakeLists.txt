# Minimum CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(WebAudioClient)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to choose target architecture
option(TARGET_ARM "Build for ARM architecture" OFF)

# If building for ARM, require a toolchain file
if(TARGET_ARM)
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake" CACHE FILEPATH "Default toolchain file for ARM builds")
        message(STATUS "No toolchain file specified; using default: ${CMAKE_TOOLCHAIN_FILE}")
    endif()
    message(STATUS "Building for ARM using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
else()
    message(STATUS "Building for x86 native architecture.")
endif()

# Ensure pkg-config is available
find_package(PkgConfig REQUIRED)

# Find required packages
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
pkg_check_modules(OPUS REQUIRED opus)
find_package(WEBSOCKETPP REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Include directories
include_directories(
    ${OPUS_INCLUDE_DIRS}
    ${WEBSOCKETPP_INCLUDE_DIRS}
    ${PORTAUDIO_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

# Link directories for opus and websocketpp (if needed)
link_directories(
    ${OPUS_LIBRARY_DIRS}
    ${WEBSOCKETPP_LIBRARY_DIRS}
    ${PORTAUDIO_LIBRARY_DIRS}
    ${JSONCPP_LIBRARY_DIRS}
)

# Add the source files
add_executable(WebAudioClient
    src/main.cc
    src/AudioProcess.cc
    src/WebsocketClient.cc
    src/Application.cc
    src/StateMachine.cc
)

# Link libraries for opus and websocketpp
target_link_libraries(WebAudioClient
    ${OPUS_LIBRARIES}
    ${WEBSOCKETPP_LIBRARIES}
    ${PORTAUDIO_LIBRARIES}
    ${JSONCPP_LIBRARIES}
)

# Custom command to clean up generated files
add_custom_target(clean-all
    #COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMAND find "${CMAKE_BINARY_DIR}" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
    COMMENT "Cleaning all generated files."
)
