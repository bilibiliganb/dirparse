#!/bin/bash

# 检查当前目录中是否存在test.db文件
if [ ! -f ./test.db ]; then
    # 如果不存在，执行xxx操作。在这个例子中，我们创建一个新的test.db文件
    dos2unix dir.txt
    ./bar
    ./searchall $1
else
    echo "search $1"
    ./searchall $1
fi
