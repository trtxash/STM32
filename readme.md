# STM32单片机

# 总纲

    目的：   
        1. 提高自学能力
        2. 掌握STM32单片机
    手段：
        1. 规范学习,理清学习;规范做到总纲到细纲,多打备注
        2. 主要自学,次要问人;自学主要看网课,如百度,B站和一些论坛

# 文件:readme
    功能：
        1. 记下学习那些事
    目的：
        1. 用VSCode搭建STM32环境,方便理解本质,方便使用
        2. 作为思路模板,不断改进;

---

## 搭建步骤
1.  下载并安装
    1.  vs code
    2.  下载并安装 STM32CubeMX ，支持最新的HAL库，工程代码配置与生成工具，支持生成IAR、Keil、STM32CubeIDE、Makefile等工程，这里使用其生成的Makefile工程。
    3.  下载并安装 Git for Windows，该工具集成有精简版的mingw，这里我们使用其bash终端和版本管理均非常有用。
    4.  下载并安装 [arm-none-eabi-gcc编译器](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)，GUN的arm的通用交叉编译链工具，基本上常用的arm处理器均支持；安装时勾选设置全局环境变量以便于配置；使用离线免安装包时，解压到合适的位置，在系统环境变量添加\bin目录，运行CMD或者Windows PowerShell，测试一下是否可用。命令：arm-none-eabi-gcc -v
    5.  下载并安装 mingw，MinGW 的全称是：Minimalist GNU on Windows 。它实际上是将经典的开源 C语言 编译器 GCC 移植到了 Windows 平台下，并且包含了 Win32API 和 MSYS，因此可以将源代码编译生成 Windows 下的可执行程序，又能如同在 Linux 平台下时，使用一些 Windows 不具备的开发工具。一句话来概括：MinGW 就是 GCC 的 Windows 版本 。其安装一般为在线安装，按网上步骤即可。  这里我们主要需要使用其 mingw32-make 功能。
    6.  下载并安装（可选） OpenOCD for Windows，一个开源的片上调试器(Open On-Chip Debugger)。linux可以直接 apt install openocd。
    7.  Jlink、ST-Link驱动（可选），如果使用OpenOCD作为GDB Server，好处是支持各种不同的调试器。但如果你仅使用Jink，不用其他的调试器，那么显然用Jlink自己的GDB Server是更加合适的。SEGGER为jlink提供了各种工具，例如J-Scope，RTT(Real Time Transfer)等。今天的内容就是把RTT Client嵌入到VS Code中。    
        1.  为什么需要日志，虽然利用断点、调用堆栈和变量窗口能很方便地进行调试，但是在一些情况下，是不允许打断点的，例如无人机、蓝牙/WiFi协议栈等。一旦程序暂停，就会造成连接断开、坠机等事故。因此，日志的输出也是很重要的调试手段，它不仅能够输出数据、还能够给我们提供程序跳转的信息。
        2.  日志的输出方法
            1.  串口输出    
            在刚开始学习STM32时，有些教程会教你把fputc函数映射到串口输出上，这样就可以使用printf函数从串口输出日志了。但是这样要占用一个串口资源、而且映射的串口发送函数是阻塞的，如果波特率太低或者发送数据太多，就会导致你的程序卡顿。
            1.  半主机  
            另一种日志输出方案是半主机（Semihosting），它是ARM调试的一种机制，利用软件中断，让嵌入式设备在调试时使用调试主机（PC）的显示器和键盘进行标准输入和输出。这样的好处是省一个串口，坏处是速度太慢，大概10ms才能出一个字符......
            1.  SWO
            Cortex-M3/M4内核的调试组件中有一个专用的模块——仪器化跟踪单元（ITM），它的作用就是输出调试信息。利用JTAG的SWO引脚可以进行输出，速度比较快。缺点是要占用一个SWO引脚，现在很多买的调试器都是SWD接口，只有SWDIO和SWCLK引脚了，带SWO的要么贵、要么太大看着笨重。
            1.  SEGGER RTT
            SEGGER RTT (Real-Time Transfer)结合了以上所有方法的优点：无需额外引脚、无需占用外设、只需要SWD引脚就可以输出日志，并且速度快，不影响应用程序的实时性。  
            想想只用插一个Jlink就可以同时断点调试+输出日志，还是很爽的。
        3. 安装SEGGER Jlink驱动全家桶
            为了使用RTT，我们需要先安装jlink驱动全家桶，这里给出：[SEGGER](https://www.segger.com/downloads/jlink)    
            找到这个jlink大礼包，点击Click for downloads，选择你的平台（windows,linux,osx）。下载完后安装即可。     
            + 如果你看了第一篇文章：[用VS Code开发STM32（一）——软件安装](https://zhuanlan.zhihu.com/p/61519415)，安了    Zadig并且修改了jlink驱动，建议直接在设备管理器里面卸载这个修改过的驱动，然后再安装jlink大礼包。
            安装完毕后你的电脑里会多出一堆工具    
            然后，把这些工具的路径加入到“Path”环境变量中：
        4. 给代码中添加源文件和头文件
            在C:\Program Files\SEGGER\JLink\Samples\RTT路径中，有一个Jlink RTT例程源码压缩包，我们把它复制到桌面再解压。然后把压缩包里的整个RTT文件夹复制到你的STM32工程中
        5. 还有一些代码的修改在后面会写下
    8.  STM32CubeProg 用于stm32下载程序
2.  配置软件
    1.  用STM32CubeMX创建工程(不能有中文！)
        1.  安装支持包，主页右边可以安装不同芯片的支持包，例如F1系列，F4系列
        2.  新建项目以后，SYS里选择debug接口，这里选的是SWD，也可以选JTAG
        3.  先在RCC里选择高速外部时钟（HSE）和低速外部时钟源（LSE），这里选的都是晶振（因为板子上有这两个晶振）。然后选择“时钟配置”，先在左边填好外部晶振的频率，然后在右边填上自己想要的主频，Cube会自动帮你配置锁相环。
        4.  Toolchain/IDE选择生成makefile即可。前面的项目结构我选的Advanced，你也可以选basic，后面目录结构就不一样了，VSCode的配置要稍微改一下。
        5.  Code Generator界面勾选Generated files全部，点击右上角的GENERATE CODE生成代码。
        6.  .ioc文件和.mxproject文件是STM32Cube的工程文件，Driver里是STM32和ARM CMSIS的库，最好不要修改。Core里面Inc和Src是供用户修改的源码。
    2.  配置VS Code
        1.  VS Code搭建所需见，[VS-Code-C](https://github.com/TRTX-gamer/VS-Code-C/blob/main/README.md)
        2.  安装VS Code 插件
            1.  Cortex-Debug：核心，有了它，才能把ARM工具链和OpenOCD等命令行工具组织到VSCode中，并进行图形化操作；
            2.  Chinese (Simplified)：VS Code的语言支持是以插件形式存在的，需要装个中文插件；
            3.  C/C++：提供代码补全、智能感知和debug功能；
            4.  ARM：提供ARM汇编语言的代码高亮，arm汇编语言支持；
        3. 配置VS Code内置终端，终端界面选择加号右边下箭头，选择默认配置文件，选择为git bash，因为git bash兼容性最好。这是全局设置。
        4.  配置智能感知，其实这个时候我们敲make已经可以编译成功了。但是VS Code的编辑窗口里会给我们亮一堆红点，代码里给我们一堆红色波浪线。这是因为VS Code本身的一个待改进的地方，下面具体解决。还记得我们使用Keil开发时，Project Options里的全局宏定义吗？在通过Makefile组织的项目中，这两个宏是通过gcc的-D参数在编译时添加的。但是，VS Code只是一个编辑器，它检查代码的时候并不会去读makefile，而是只看.h和.c文件，于是stm32f4xx.h中就检测不到那个宏，表现为灰色（认为这个宏没有被定义），于是你就可以看到一大串”xxxx is undefined”之类的报错，其实都是这个原因导致的。但是直接去make的话是没有问题的。因此我们需要在当前目录的.vscode文件夹下创建c_cpp_properties.json配置文件，用来告诉VS Code我们定义了这些宏。随便找到一处红色波浪线，点击并把光标移到那一行，左上角会出现一个黄色小灯泡。点击黄色小灯泡并选择“编辑‘includePath设置’”。直接用c_cpp_properties.json来配置，VS Code自动在当前目录下的.vscode文件夹下生成一个c_cpp_properties.json文件，我的配置在下面给出。这个配置是我研究测试很久得到的，保证没问题，下面详细解释几个重要部分：
            1.  “name”：这是用于标记使用的平台的标签。除了win32还可以选Linux或Mac。也就是说，这个json里“configuration“下可以写三组配置，只要每组配置前面写上不同的平台，即可在不同的操作系统上使用就会自动适配不同的配置，非常方便
            2.  “includePath”：告诉VS Code该去哪里查找头文件。第一个目录是C语言标准库的目录， 剩下的几个目录直接从Makefile里复制然后稍微修改下即可。"${workspaceFolder}"表示项目文件夹；
            3.  “defines”：全局宏定义，告诉VS Code这些宏都被定义了，只是没写在源码中而已。上述多加的两个宏是makefile里的。
            4.  "compilerPath"：指定编译器的路径。因为有一些宏是编译器自带的，连makefile里都没有，例如__GNUC__。有些教程里会让你在defines里面加上__GNUC__，但是这是没必要的。只要你指定了编译器路径，所有的编译器自带的宏就都导入了VS Code。
            5.  "intelliSenseMode"：因为我们用的是gcc所以选gcc-x64
            6.  "browse.path"：源文件搜索路径。据说是用来做代码补全和查找定义的，但是我测试后发现删去也不影响使用，不过还是留着吧。这个路径和includePath不同，browse.path是自动递归所有子目录的。而include.path默认只看本目录。
            7.  Ctrl+S保存c_cpp_properties.json文件，发现左边目录里一个红点都没有了，强迫症舒服了！
            8.  如果还是有红点和波浪线，检查c_cpp_properties.json文件，检查stm32f1xx_hal_conf.h文件是否禁用了用到的头文件。
    3.  调试
        1.  跳过OpenOCD，如果需要看[这里](https://zhuanlan.zhihu.com/p/61541590)
        2.  给代码中添加源文件和头文件，在C:\Program Files (x86)\SEGGER\JLink\Samples\RTT路径中，有一个Jlink RTT例程源码压缩包，我们把它复制到桌面再解压。然后把压缩包里的整个RTT文件夹复制到你的STM32工程中
        3.  添加测试程序、修改Makefile和c_cpp_properties.json
            1.  自己看着RTT的文件在程序中添加测试（可以跳过）
            2.  修改Makefile，添加头文件路径和源文件
            3.  修改c_cpp_properties.json
            4.  添加launch.json，在运行和调试中选择自己的工程文件夹的添加配置，生成了launch.json，照着下面的例子改动一些
            5.  添加tasks.json，直接在.vscode里面添加，照着下面的例子改动一些,这个文件创建了两个bash任务，分别叫做build和clean，build任务就是在bash里执行了mingw32-make，clean任务就是在bash里执行了mingw32-make clean

---
> c_cpp_properties.json配置文件，网上抄的  
/* 具体根据自己安装位置更改一点,有红线就是这个文件没配置好 */
```
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/lib/gcc/arm-none-eabi/10.3.1/include",
                "${workspaceFolder}/Core/Inc",
                "${workspaceFolder}/Drivers/STM32F1xx_HAL_Driver/Inc",
                "${workspaceFolder}/Drivers/CMSIS/Device/ST/STM32F1xx/Include",
                "${workspaceFolder}/Drivers/CMSIS/Include"
            ],
            "defines": [
                "USE_HAL_DRIVER",
                "STM32F103xE"  
            ],
            "compilerPath": "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-gcc.exe",
            "intelliSenseMode": "gcc-x64",
            "browse": {
                "limitSymbolsToIncludedHeaders": true,
                "databaseFilename": "",
                "path": [
                    "${workspaceFolder}"
                ]
            }
        }
    ],
    "version": 4
}
```
---

> c_cpp_properties.json配置文件，自己的  
/* 具体根据自己安装位置更改一点,有红线就是这个文件没配置好 */
```
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/lib/gcc/arm-none-eabi/10.3.1/include",
                "${workspaceFolder}/Core/Inc",
                "${workspaceFolder}/Drivers/STM32F1xx_HAL_Driver/Inc",
                "${workspaceFolder}/Drivers/CMSIS/Device/ST/STM32F1xx/Include",
                "${workspaceFolder}/Drivers/CMSIS/Include",
                "${workspaceFolder}/RTT",
                "${workspaceFolder}/User/Inc"
            ],
            "defines": [
                "USE_HAL_DRIVER",
                "STM32F103xE",
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "compilerPath": "C:/Program Files (x86)/GNU Arm Embedded Toolchain/10 2021.10/bin/arm-none-eabi-gcc.exe",
            "cStandard": "c99",
            "cppStandard": "gnu++98",
            "intelliSenseMode": "windows-gcc-arm64"
        }
    ],
    "version": 4
}
```
---

> launch.json
```
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Cortex Debug", // 配置名称，将会在启动配置的下拉菜单中显示
            "cwd": "${workspaceFolder}", // 调试程序时的工作目录，此为工作区文件夹；改成${fileDirname}可变为文件所在目录
            "executable": "${workspaceFolder}/build/Template.elf", // 编译出的二进制文件，也就是最终烧录到单片机中的，这里是elf文件。根据芯片的不同，可能产生不同的名称和后缀（例如TI的TM4C123芯片编译出来的名称是"main.axf"）
            "request": "launch", // 请求配置类型，可以为launch（启动）或attach（附加）
            "type": "cortex-debug", // 配置类型，是cortex-debug类型配置，其实也可以填cppdbg之类的，但是那样我们就得自己配置gdb了，配置起来将会非常麻烦。
            "runToEntryPoint": "main",
            "servertype": "jlink",
            "device": "STM32F103ZE", // 使用J-link GDB Server时必须有；其他GBD Server时可选（有可能帮助自动选择SVD文件）。支持的设备见 https://www.segger.com/downloads/supported-devices.php
            "svdFile": "./STM32F103.svd", // svd文件，有这个文件才能查看寄存器的值，每个单片机都不同。可以在以下地址找到 https://github.com/posborne/cmsis-svd
            "preLaunchTask": "build" // 调试会话开始前执行的任务，一般为编译程序。与tasks.json的label相对应
        }
    ]
}
```
---

> tasks.json
```
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "mingw32-make",
            "args": []
        },
        {
            "label": "clean",
            "type": "shell",
            "command": "mingw32-make",
            "args": [
                "clean"
            ]
        },
        {
            "label": "rtt",
            "type": "shell",
            "command": "JLinkRTTClient",
            "args": []
        }
    ]
}
```
---

> [vscode中的${workspaceFolder}等变量](https://blog.csdn.net/weixin_39249524/article/details/105438814)
```
${workspaceFolder} :表示当前workspace文件夹路径，也即/home/Coding/Test
${workspaceRoot} 当前打开的文件夹的绝对路径+文件夹的名字
${workspaceRootFolderName}:表示workspace的文件夹名，也即Test，当前打开的文件夹的名字
${file}:文件自身的绝对路径，也即/home/Coding/Test/.vscode/tasks.json,有人说是/*.cpp，没试过
${relativeFile}:文件在workspace中的路径，也即.vscode/tasks.json
${fileBasenameNoExtension}:当前文件的文件名，不带后缀，也即tasks
${fileBasename}:当前文件的文件名，tasks.json
${fileDirname}:文件所在的文件夹路径，也即/home/Coding/Test/.vscode
${fileExtname}:当前文件的后缀，也即.json
${lineNumber}:当前文件光标所在的行号
${env:PATH}:系统中的环境变量
```
---
> 先先看一下什么是makefile
```
makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作，因为 makefile就像一个Shell脚本一样，其中也可以执行操作系统的命令。makefile带来的好处就是--“自动化编译”,一旦写好，只需要一个make命令，整个工程完全自动编译，极大的提高了软件开发的效率。make是一个命令工具，是一个解释makefile中指令的命令工具，一般来说，大多数的IDE都有这个命令，比如：Delphi的make,Visual C++的nmake,Linux下GNU的make.可见，makefile都成为了一种在工程方面的编译方法。
make
    根据Makefile文件编译源代码、连接、生成目标文件、可执行文件。
make clean
    清除上次的make命令所产生的object文件（后缀为“.o”的文件）及可执行文件。
make install
    将编译成功的可执行文件安装到系统目录中，一般为/usr/local/bin目录。
make dist
    产生发布软件包文件（即distribution package）。这个命令将会将可执行文件及相关文件打包成一个tar.gz压缩的文件用来作为发布软件的软件包。
    它会在当前目录下生成一个名字类似“PACKAGE-VERSION.tar.gz”的文件。PACKAGE和VERSION,是我们在configure.in中定义的AM_INIT_AUTOMAKE（PACKAGE, VERSION）。
make distcheck
    生成发布软件包并对其进行测试检查，以确定发布包的正确性。这个操作将自动把压缩包文件解开，然后执行configure命令，并且执行make,来确认编译不出现错误，最后提示你软件包已经准备好，可以发布了。
make distclean
    类似make clean,但同时也将configure生成的文件全部删除掉，包括Makefile文件。
```
---

## 参考资料
两本参考资料，即《STM32 中文参考手册》和《Cortex-M3 权威指南》。    
《STM32 中文参考手册》是 ST 出的官方资料，有 STM32 的详细介绍，包括了 STM32 的各种寄存器定义以及功能等，是学习 STM32 的必备资料之一。而《Cortex-M3 权威指南》则是对《STM32 中文参考手册》的补充，后者一般认为使用STM32 的人都对CM3 有了较深的了解，所以 Cortex-M3 的很多东西它只是一笔带过，但前者对 Cortex-M3 有非常详细的说明。

---

## 技巧
先学HAL库版本再学寄存器版本，完全理解怎样使用。掌握时钟树图，见《STM32 中文参考手册》
> 注释小技巧
```
文件注释
@file：             文件名，xx.c; zz.h等

@brief              简介

@author             作者

@email              邮箱

@version            版本号

@date               日期

@license            许可证

@copy/@attention    详细描述

描述：用以说明整个文件的各种信息。
```
```
函数注释
@brief      对函数简要描述

@param      参数说明，以’:’作为参数结束标志；

@arg        参数里面可选择参量列举，对于可数情况可进行参量列举，同样以’:’作为参数结束标志；

@note       注释，配合brief一起使用可以达到很好的注释效果；

@retval     返回值说明。

描述：对函数体进行说明，包括功能，参数和返回值。
```
---
### 备注
+ 我使用的是正点原子STM32F103ZET6开发板
+ [用VS Code开发STM32（一）——软件安装](https://zhuanlan.zhihu.com/p/61519415)
+ [用VS Code开发STM32（二）——编译](https://zhuanlan.zhihu.com/p/61538230)
+ [用VS Code开发STM32（三）——调试](https://zhuanlan.zhihu.com/p/61541590)
+ [用VS Code开发STM32（四）——增加SEGGER RTT日志输出支持](https://zhuanlan.zhihu.com/p/163771273)
+ [VS Code 搭建stm32开发环境](https://www.cnblogs.com/silencehuan/p/11815263.html)
---

创于:2022年2月10日16点37分