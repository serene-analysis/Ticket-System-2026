#!/bin/bash

# 定义build目录路径（适配你的项目结构）
BUILD_DIR="."

# 1. 进入build目录（如果目录不存在则报错退出）
cd "$BUILD_DIR" || {
    echo "错误：build目录不存在！"
    exit 1
}

# 2. 删除build目录下所有.data文件（-f避免文件不存在时报错）
rm -f *.data
echo "已删除build目录下的所有.data文件"

# 3. 运行可执行文件（若程序需要参数，可在./bookstore后添加）
make -j4
./code