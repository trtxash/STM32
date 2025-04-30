# STM32单片机 总纲

功能

1. 记下学习那些事

---

## 搭建步骤

之前版本基于GCC ARM和VScode官方C插件，现在更换为clang和cmaketool  
见：[超级简单的 STM32 + VSCode + clangd + Makefile/CMake 开发](https://summonhim.github.io/p/%E8%B6%85%E7%BA%A7%E7%AE%80%E5%8D%95%E7%9A%84-stm32--vscode--clangd--makefile/cmake-%E5%BC%80%E5%8F%91/)  
和：[linkyourbin](https://github.com/linkyourbin/cmake_ninja_clangd_stm32_dev_env),[linkyourbin-bilibili](https://space.bilibili.com/3493142393260061?spm_id_from=333.337.0.0)

1. STM32CubeMX创建工程，IDE选cmake，整理库
2. 下载安装[LLVM](https://releases.llvm.org/)，[Cmake](https://cmake.org/)，[ninja](https://ninja-build.org/)，[GNU-ARM(gcc-arm-none-eabi)](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)等，添加好环境变量
3. 下载安装cursor，调整AI模型和主题等
4. cursor安装cmaketool、clangd、cortex-Debug等插件，注：camke tool插件不兼容C/C++插件
5. 建立.vscode文件夹里面settings.json设置clangd

    >.settings.json

    ```json
    {
        "clangd.arguments": [
            "--query-driver=D:/Works/Arm GNU Toolchain arm-none-eabi/bin/arm-none-eabi-gcc.exe", # 指定编译器
            "--compile-commands-dir=${workspaceFolder}/build/Debug" # 指定cmake编译辅助文件输出目录，给clangd用
        ]
    }
    ```

6. 建立.clangd文件设置clangd

    >.clangd

    ```yaml
    CompileFlags:
    Add:
        [
            --target=arm-none-eabi,
            -ID:/Works/Arm GNU Toolchain arm-none-eabi/arm-none-eabi/include,  # ARM-GCC头文件路径
            -I/Works/Arm GNU Toolchain arm-none-eabi/lib/gcc/arm-none-eabi/14.2.1/include,  # GCC库头文件
            -ferror-limit=500
        ]
    Compiler: arm-none-eabi-gcc
    Diagnostics:
    MissingIncludes: None # 忽视错误
    UnusedIncludes: None # 忽视错误
    ```

    可选：建立.clang-format文件设置格式化标准

    >.clang-format

    ```yaml
    BasedOnStyle: Microsoft
    AccessModifierOffset: -4 # 访问修饰符（public/private/protected）相对类体的缩进偏移量为-4空格
    AlignConsecutiveMacros: true # 自动对齐连续的宏定义
    AlignTrailingComments: true # 自动对齐尾部注释
    AllowShortFunctionsOnASingleLine: Inline # 允许内联短函数单行显示
    # AllowShortIfStatementsOnASingleLine: false # 禁止短if语句单行显示
    # BreakBeforeBraces: Allman # 大括号换行风格（Allman风格）
    # ColumnLimit: 0 # 禁用行宽限制（不自动换行）
    ```

7. 下载jlink，.vscode下设置tasks.json和launch.json

    >tasks.json

    ```json
    {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "CMake: config and build",
                "dependsOrder": "sequence", // 按顺序执行
                "dependsOn": [
                    "CMake: configure",
                    "CMake: build"
                ],
                "group": "build"
            }
        ]
    }
    ```

    >launch.json

    ```json
    {
        // 使用 IntelliSense 了解相关属性。 
        // 悬停以查看现有属性的描述。
        // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
        "version": "0.2.0",
        "configurations": [
            {
                "name": "Cortex Debug-jlink", // 配置名称，将会在启动配置的下拉菜单中显示
                "cwd": "${workspaceFolder}", // 调试程序时的工作目录，此为工作区文件夹；改成${fileDirname}可变为文件所在目录
                "executable": "${workspaceFolder}/build/Debug/template.elf", // 编译出的二进制文件，也就是最终烧录到单片机中的，这里是elf文件。根据芯片的不同，可能产生不同的名称和后缀（例如TI的TM4C123芯片编译出来的名称是"main.axf"）
                "request": "launch", // 请求配置类型，可以为launch（启动）或attach（附加）
                "type": "cortex-debug", // 配置类型，是cortex-debug类型配置，其实也可以填cppdbg之类的，但是那样我们就得自己配置gdb了，配置起来将会非常麻烦。
                "runToEntryPoint": "main",
                "servertype": "jlink",
                "device": "STM32F429ZG", // 使用J-link GDB Server时必须有；其他GBD Server时可选（有可能帮助自动选择SVD文件）。支持的设备见 https://www.segger.com/downloads/supported-devices.php
                "svdFile": "./STM32F429.svd", // svd文件，有这个文件才能查看寄存器的值，每个单片机都不同。可以在以下地址找到 https://github.com/posborne/cmsis-svd
                "preLaunchTask": "CMake: config and build", // 调试会话开始前执行的任务，一般为编译程序。与tasks.json的label相对应
                "showDevDebugOutput": "raw",
            },
            {
                "name": "Cortex Debug-openocd", // 配置名称，将会在启动配置的下拉菜单中显示
                "cwd": "${workspaceFolder}", // 调试程序时的工作目录，此为工作区文件夹；改成${fileDirname}可变为文件所在目录
                "executable": "${workspaceFolder}/build/Debug/template.elf", // 编译出的二进制文件，也就是最终烧录到单片机中的，这里是elf文件。根据芯片的不同，可能产生不同的名称和后缀（例如TI的TM4C123芯片编译出来的名称是"main.axf"）
                "request": "launch", // 请求配置类型，可以为launch（启动）或attach（附加）
                "type": "cortex-debug", // 配置类型，是cortex-debug类型配置，其实也可以填cppdbg之类的，但是那样我们就得自己配置gdb了，配置起来将会非常麻烦。
                "runToEntryPoint": "main",
                "servertype": "openocd",
                "device": "STM32F429ZG", // 使用J-link GDB Server时必须有；其他GBD Server时可选（有可能帮助自动选择SVD文件）。支持的设备见 https://www.segger.com/downloads/supported-devices.php
                "svdFile": "./STM32F429.svd", // svd文件，有这个文件才能查看寄存器的值，每个单片机都不同。可以在以下地址找到 https://github.com/posborne/cmsis-svd
                "configFiles": [
                    "C:/OpenOCD-20231002-0.12.0/share/openocd/scripts/interface/cmsis-dap.cfg",
                    "C:/OpenOCD-20231002-0.12.0/share/openocd/scripts/target/stm32f4x.cfg"
                ],
                "preLaunchTask": "CMake: config and build", // 调试会话开始前执行的任务，一般为编译程序。与tasks.json的label相对应
                "showDevDebugOutput": "raw",
            }
        ]
    }
    ```

8. F5测试调试功能。注意：网络代理可能导致GDB连接失败
9. 待完善（clangd警告问题）

---

### 备注

+ [用VS Code开发STM32（一）——软件安装](https://zhuanlan.zhihu.com/p/61519415)
+ [用VS Code开发STM32（二）——编译](https://zhuanlan.zhihu.com/p/61538230)
+ [用VS Code开发STM32（三）——调试](https://zhuanlan.zhihu.com/p/61541590)
+ [用VS Code开发STM32（四）——增加SEGGER RTT日志输出支持](https://zhuanlan.zhihu.com/p/163771273)
+ [VS Code 搭建stm32开发环境](https://www.cnblogs.com/silencehuan/p/11815263.html)
+ [MINIF401](https://github.com/WeActTC/MiniSTM32F4x1)
+ [linkyourbin](https://github.com/linkyourbin/cmake_ninja_clangd_stm32_dev_env)
+ [linkyourbin-bilibili](https://space.bilibili.com/3493142393260061?spm_id_from=333.337.0.0)

---

>更新记录  
2025年4月23日20点49分：创建  
2025年4月30日16点09分：完善文档
