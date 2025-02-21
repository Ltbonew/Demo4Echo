## LVGL demo

### 1.编译
1. **在电脑上运行SDL仿真**
    ```sh
    cd ./lvgl_demo
    mkdir ./build
    cd ./build
    cmake ..
    make
    ```

    然后就可以在ubuntu上运行了

    ```sh
    ../bin/main
    ```

2. **编译到开发板上运行**

    ```sh
    cd ./build
    cmake .. -DTARGET_ARM=ON
    make
    ```

    然后把可执行文件发送到开发板就能运行了

### 2.注意

每个page可视为一个APP, 想要自行添加可以参考模版添加