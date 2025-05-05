@echo off
for /f "delims=" %%i in ('dir /s /b /ad ,.cache,build 2^>nul') do (
    rd /s /q "%%i"
)


@REM for /f "delims=" %%i in (...) do
@REM /f 表示处理文件/命令输出
@REM "delims=" 禁用默认的分隔符（防止路径中的空格被切割）
@REM %%i 循环变量，保存找到的每个路径

@REM 'dir /s /b /ad .vscode 2^>nul'
@REM dir 查找目录命令
@REM /s 包含子目录
@REM /b 简洁格式（只显示完整路径）
@REM /ad 只搜索目录（排除文件）
@REM .vscode 目标文件夹名称
@REM 2^>nul 将错误输出重定向到空设备（隐藏报错信息）

@REM rd /s /q "%%i"
@REM rd 删除目录命令（rmdir的简写）
@REM /s 递归删除子目录和文件
@REM /q 静默模式（无需确认）

exit