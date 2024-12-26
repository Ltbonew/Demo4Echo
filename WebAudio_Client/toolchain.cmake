# Specify the cross-compilation toolchain
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the compiler paths
set(CMAKE_C_COMPILER /path/to/arm-gcc)
set(CMAKE_CXX_COMPILER /path/to/arm-g++)

# Specify the sysroot (if available)
set(CMAKE_SYSROOT /path/to/sysroot)

# Add paths to find libraries and includes
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)