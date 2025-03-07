## DeskBot demo

### 1.编译
1. **在电脑上运行SDL仿真**

    注意更改conf/dev_conf, 将`LV_USE_SIMULATOR`置`1`
    
    ```sh
    cd ./lvgl_demo
    mkdir ./build
    cd ./build
    cmake ..
    make
    ```

    然后就可以在ubuntu上运行了

    ```sh
    cd ../bin
    ./main
    ```

2. **编译到开发板上运行**

    注意更改conf/dev_conf, 将`LV_USE_SIMULATOR`置`0`

    ```sh
    cd ./build
    cmake .. -DTARGET_ARM=ON
    make
    ```

    然后把可执行文件所在的文件夹`/bin`, copy到开发板, 在开发板上就能运行了

    **注意:** 

    ```sh
    cd ../bin
    ./main
    ```

### 2.注意

1. `./bin/system_para.conf`中有高德API key，需要换成你的API key，用于天气等数据的获取

2. 每个page可视为一个APP, 想要自行添加可以参考模版添加