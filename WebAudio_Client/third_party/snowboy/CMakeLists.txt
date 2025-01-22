cmake_minimum_required(VERSION 3.10)

# Option to choose target architecture
option(TARGET_ARM "Build for ARM architecture" OFF)

# Find the architecture
if(TARGET_ARM)
    set(SNOWBOY_LIBRARY "${CMAKE_SOURCE_DIR}/lib/rpi/libsnowboy-detect.a")
    message(STATUS "Building for Arm native architecture.")
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake" CACHE FILEPATH "Default toolchain file for ARM builds")
        message(STATUS "No toolchain file specified; using default: ${CMAKE_TOOLCHAIN_FILE}")
    endif()
    message(STATUS "Building for ARM using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
else()
    set(SNOWBOY_LIBRARY "${CMAKE_SOURCE_DIR}/lib/ubuntu64/libsnowboy-detect.a")
    message(STATUS "Building for x86 native architecture.")
endif()

# Set the project name
project(DemoSnowboy)

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Add executable
add_executable(demo2 demo2.cc)
add_executable(demo1 demo1.cc AudioProcess.cc)

# Link libraries
target_link_libraries(demo2 ${SNOWBOY_LIBRARY})
target_link_libraries(demo1 ${SNOWBOY_LIBRARY})

# Additional flags and options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

# 确保链接必要的数学库
if(TARGET_ARM)
    # 对于 ARM 目标，明确查找 OpenBLAS
    find_package(OpenBLAS REQUIRED)

    if(OpenBLAS_FOUND)
        set(BLAS_LIBRARIES ${OpenBLAS_LIBRARIES})
        set(LAPACK_LIBRARIES ${OpenBLAS_LIBRARIES})  # OpenBLAS 包含 LAPACK 实现
        set(BLAS_INCLUDE_DIRS ${OpenBLAS_INCLUDE_DIR})
    endif()
    pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
    pkg_check_modules(OPUS REQUIRED opus)
else()
    # 对于非 ARM 目标，使用通用的 BLAS 和 LAPACK 查找
    find_package(BLAS REQUIRED)
    find_package(LAPACK REQUIRED)
    pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)
    pkg_check_modules(OPUS REQUIRED opus)
endif()
# 将找到的库添加到目标
include_directories(${BLAS_INCLUDE_DIRS})
target_link_libraries(demo2 ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
target_link_libraries(demo1 ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${PORTAUDIO_LIBRARIES} ${OPUS_LIBRARIES})

add_custom_target(clean-all
    COMMAND find "${CMAKE_BINARY_DIR}" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
    COMMENT "Cleaning all generated files."
)