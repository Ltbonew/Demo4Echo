
file(GLOB_RECURSE GUI_APP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"  # 查找当前目录及子目录中的所有 .c 文件
)

# 定义 utils 模块为一个静态库
add_library(gui_app STATIC ${GUI_APP_SOURCES})

# 设置 utils 的头文件路径
target_include_directories(gui_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
