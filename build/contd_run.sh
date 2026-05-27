#!/bin/bash

# 定义build目录路径（适配你的项目结构）
BUILD_DIR="."

# 1. 进入build目录（如果目录不存在则报错退出）
cd "$BUILD_DIR" || {
    echo "错误：build目录不存在！"
    exit 1
}

# 3. 运行可执行文件（若程序需要参数，可在./bookstore后添加）
make -j4
./code < test.in > test.out