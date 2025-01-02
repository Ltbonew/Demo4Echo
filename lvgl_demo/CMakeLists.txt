cmake_minimum_required(VERSION 3.10)
project(lvgl)

set(CMAKE_C_STANDARD 99)#C99 # lvgl officially support C99 and above
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(lvgl)          # LVGL 核心模块
add_subdirectory(gui_app)       # GUI_APP 应用模块
add_subdirectory(utils)         # 公共工具模块

# Include 配置头文件路径
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(lvgl PUBLIC ${PROJECT_SOURCE_DIR}/conf)

# 定义主程序源文件
set(MAIN_SOURCES main.c)

# 链接库
add_executable(main ${MAIN_SOURCES})

# 链接自定义的文件夹库
target_link_libraries(
    main 
    utils
)

include(${CMAKE_CURRENT_LIST_DIR}/lvgl/tests/FindLibDRM.cmake)
include_directories(${Libdrm_INCLUDE_DIRS})

find_package(SDL2)
find_package(SDL2_image)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${Libdrm_LIBRARIES} m pthread)
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)

# Custom command to clean up generated files
add_custom_target(clean-all
    COMMAND find "${CMAKE_BINARY_DIR}" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
    COMMENT "Cleaning all generated files."
)