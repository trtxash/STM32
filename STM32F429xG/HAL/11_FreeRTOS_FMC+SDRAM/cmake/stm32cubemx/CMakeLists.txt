cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32F429xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../BSP/BSP_User/Inc
    ../../BSP/BSP_Core/Inc
    ../../Drivers/STM32F4xx_HAL_Driver/Inc
    ../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ../../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../../Drivers/CMSIS/Include
    ../../FreeRTOS/Inc
    ../../FreeRTOS/portable/GCC/ARM_CM4F
    ../../APP/Debug_Linker/RTT/Inc
    ../../Drivers_Hardware/LED/Inc
    ../../Drivers_Hardware/SDRAM/Inc
)

target_sources(stm32cubemx INTERFACE
    ../../BSP/BSP_User/Src/main.c
    ../../BSP/BSP_User/Src/stm32f4xx_it.c
    ../../BSP/BSP_User/Src/stm32f4xx_hal_msp.c
    ../../BSP/BSP_User/Src/system_stm32f4xx.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
    ../../FreeRTOS/Src/tasks.c
    ../../FreeRTOS/Src/list.c
    ../../FreeRTOS/Src/timers.c
    ../../FreeRTOS/Src/queue.c
    ../../FreeRTOS/portable/GCC/ARM_CM4F/port.c
    ../../FreeRTOS/portable/MemMang/heap_4.c
    ../../BSP/BSP_User/Src/sys.c
    ../../BSP/BSP_User/Src/delay.c
    ../../BSP/BSP_User/Src/bsp_app.c
    ../../BSP/BSP_User/Src/bsp_freertos.c
    ../../BSP/BSP_Core/Src/fmc.c
    ../../BSP/BSP_Core/Src/tim.c
    # ../../BSP/BSP_Core/Src/iwdg.c
    # ../../BSP/BSP_Core/Src/wwdg.c
    ../../Drivers_Hardware/LED/Src/led.c
    ../../Drivers_Hardware/SDRAM/Src/sdram.c
    ../../APP/Debug_Linker/RTT/Src/SEGGER_RTT.c
    ../../APP/Debug_Linker/RTT/Src/SEGGER_RTT_printf.c
    ../../APP/Debug_Linker/RTT/SEGGER_RTT_ASM_ARMv7M.S
    ../../startup_stm32f429xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


