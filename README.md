##### 一、linux开发环境部署

* 安装`gcc-arm-none-eabi`（已包含`binutils`）
* 配置国内镜像库
* 安装`cmake`
* 安装`USB`库`sudo apt-get install libusb-1.0-0 libusb-1.0-0-dev`
* 安装自定义内核，以支持`wsl usb device`
* 安装`jlink`驱动，jlink官网先下载deb文件[https://www.segger.com/downloads/jlink/](https://www.segger.com/downloads/jlink/)，执行`sudo dpkg -i JLink_Linux_V810c_x86_64.deb`，安装依赖`sudo apt-get install -f`
* 查看JLINK状态，`JLinkExe`
* `windows`安装`usbipd`挂载`jlink`到`wsl`上
* 安装`openocd`并配置`openocd`

  `openocd -f /mnt/c/xpack-openocd-0.12.0-1/openocd/scripts/interface/jlink.cfg -f /mnt/c/xpack-openocd-0.12.0-1/openocd/scripts/target/stm32g0x.cfg`
* 配置`gdb`脚本

  1. 选择elf文件进行gdb调试，`arm-none-eabi-gdb G070.elf`
  2. 连接openocd，`(gdb)target remote localhost:3333（STLINK为4242端口）`
  3. 重新load elf文件，`(gdb)monitor reset；(gdb)monitor halt；(gdb)load`
  4. 进入GUI模式，`(gdb)-`

##### 二、与ARMCC不一致文件

* 由cubemx生成`cubeide项目`中的`ld`文件

  添加`cd_sec`自动初始化段、`cmbacktrace _sstack`、`lettershell init`）
* 由cubemx生成`cubeide项目`中的`cmakefilelists`文件

  添加所有`.c，.s，.h`文件，注意cmake目录下的`cmakelists`文件及`json`文件
* `startup_stm32g070xx.s`文件
* `__io_putchar`重定向实现`printf`

##### 三、打包分发ubuntu

![image](assets/image-20241023115421-ptd44go.png)

##### 四、linux下常用命令

* 查看gcc版本，`arm-none-eabi-gcc --version`
* 查看安装arm gcc编译链版本，`dpkg -l | grep gcc-arm`
* 当前目录执行cmake，`cmake .`
* build目录下执行cmake，`cmake ..`
* 清除当前目录全部文件，`rm * -rf`
* 生成lst文件，`arm-linux-gnueabi-objdump -d G070.elf > G070.lst`
* powershell窗口列出所有USB设备，`usbipd list`
* windows下JLINK挂载到linux，`usbipd attach -w --busid 1-6`
* windows下JLINK解挂载，`usbipd detach -a`
* 切换root用户，`sudo -s`
* linux重启，`sudo init 6（0为关机）`

##### 五、20241029 G070_CMAKE项目操作流程

1. 项目根目录`./build.sh new`，全量编译并下载
2. 项目根目录`./build.sh`，增量编译(不包含`.ld`及新增文件，有变化需要`./build.sh new`)
3. 项目根目录`./flash.sh`，调用jlink对已生成bin文件进行下载
4. 项目根目录`./dbg.sh`，开启`openocd`及`gnu gui`

##### 六、GDB常用命令

`list shell.c:1040`	——	查看文件
`break cd_service.c:1`	——	设定断点
`i b`					——	查看断点
`clear`				——	清除当前断点
`d 1`					——	删除1号断点

`aw task_active`		——	访问观察点，内存读/写时会停止
`rw *task_active`		——	只读观察点，内存读取时会停止
`s`					——	步进
`n`					——	跳过函数执行
`finish`				——	跳出当前函数
`u 10	`				——	直接运行到当前文件第10行

`bt`					——	栈回溯
`p *ptr`				——	打印结构体指针中所有成员值
`info reg`			——	查看栈帧中所有寄存器

`q`					——	退出gdb
`ctrl + c`			——	停止运行
