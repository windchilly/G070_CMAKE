#!/bin/bash

# 检查 OpenOCD 是否在运行
if ! pgrep -x "openocd" > /dev/null; then
    echo "OpenOCD 未运行，正在启动..."
    openocd -f /mnt/c/xpack-openocd-0.12.0-1/openocd/scripts/interface/jlink.cfg -f /mnt/c/xpack-openocd-0.12.0-1/openocd/scripts/target/stm32g0x.cfg &
    sleep 2  # 等待 OpenOCD 启动
else
    echo "OpenOCD 已经在运行。"
fi

# 进入 build 目录并启动 GDB
cd build || { echo "无法进入 build 目录"; exit 1; }
arm-none-eabi-gdb G070.elf -ex "target remote localhost:3333" -ex "monitor reset" -ex "monitor halt" -ex "load" -ex "-"

# 保留在 GDB 界面
echo "GDB 已启动并执行完初始命令，现在保留在 GDB 界面。"
