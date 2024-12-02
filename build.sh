#!/bin/bash

# 检查是否有new命令作为参数
if [ "$1" == "new" ]; then
    # 如果有new命令，删除现有的build文件夹并新建一个
    if [ -d "build" ]; then
        rm -rf build
    fi
    mkdir build
    cd build
    # 在build文件夹中执行cmake命令
    cmake ../
    
    # 检查cmake命令是否成功执行
    if [ $? -eq 0 ]; then
        echo "CMake completed successfully."
        # 执行make命令
        make
        # 检查make命令是否成功执行
        if [ $? -eq 0 ]; then
            echo "Make completed successfully."
        else
            echo "Make failed."
            exit 1
        fi
    else
        echo "CMake failed."
        exit 1
    fi
else
    # 如果没有new命令，直接进入build文件夹
    if [ -d "build" ]; then
        cd build
        # 执行make命令
        make
        # 检查make命令是否成功执行
        if [ $? -eq 0 ]; then
            echo "Make completed successfully."
        else
            echo "Make failed."
            exit 1
        fi
    else
        echo "Build directory does not exist."
        exit 1
    fi
fi

# 如果make命令执行成功，回到根目录并执行flash.sh脚本
cd ..
./flash.sh

# 检查flash.sh脚本是否成功执行
if [ $? -eq 0 ]; then
    echo "Success"
else
    echo "Flash script failed."
fi
