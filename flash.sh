#!/bin/bash

DEVICE="STM32G070RB" # 替换为你的目标设备名称
BIN_FILE="build/G070.bin" # 要烧录的bin文件路径
START_ADDRESS="0x08000000" # 烧录起始地址，根据实际情况修改

JlinkScript="./flash.jlink"

# 检查bin文件是否存在
if [ ! -f "$BIN_FILE" ]; then
  echo "The bin file does not exist." >&2
  exit 1
fi

if [ -f $JlinkScript ]; then
    rm $JlinkScript
fi

touch $JlinkScript
echo h > $JlinkScript
echo loadfile $BIN_FILE $START_ADDRESS >> $JlinkScript
echo r >> $JlinkScript
echo g >> $JlinkScript
echo exit >> $JlinkScript

# 使用JLinkExe进行烧录
# echo "Starting to flash the bin file..."
# JLinkExe -device $DEVICE -autoconnect 1 -if SWD -speed 2000 -CommanderScript $JlinkScript

# 使用JLinkExe进行烧录并捕获输出
echo "Starting to flash the bin file..."
OUTPUT=$(JLinkExe -device $DEVICE -autoconnect 1 -if SWD -speed 2000 -CommanderScript $JlinkScript 2>&1)

# 检查输出中是否包含 "O.K."
if echo "$OUTPUT" | grep -q "O.K."; then
    echo "JLINK下载成功"
else
    echo "JLINK下载失败: $OUTPUT" >&2
    exit 1
fi
