
## 语言助手demo

如何编译如下：

X86：

```sh
mkdir build && cd build
cmake ..
make
```

arm:

```sh
mkdir build && cd build
cmake -DTARGET_ARM=ON ..
make
```

清除:

```sh
# in dir: build
make clean-all
```
