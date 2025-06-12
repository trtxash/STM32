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
    ../../BSP/BSP_Tasks/Inc
    ../../BSP/BSP_Devices/Inc

    ../../Drivers/STM32F4xx_HAL_Driver/Inc
    ../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ../../Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../../Drivers/CMSIS/Include

    ../../FreeRTOS/Inc
    ../../FreeRTOS/portable/GCC/ARM_CM4F

    ../../LVGL/lvgl-9.3.0/

    ../../APP/Debug_Linker/RTT/Inc
    ../../APP/Multi_Button/Inc
    ../../APP/Multi_Led/Inc
    ../../APP/Algorithm/KalmanFilter/Inc

    ../../Drivers_Hardware/Key/Inc
    ../../Drivers_Hardware/LCD/Inc
    ../../Drivers_Hardware/FT5446/Inc
    # ../../Drivers_Hardware/OLED/Inc
    ../../Drivers_Hardware/LED/Inc
    ../../Drivers_Hardware/SDRAM/Inc
)

file(GLOB_RECURSE FREERTOS_SOURCES
    ../../FreeRTOS/Src/tasks.c
    ../../FreeRTOS/Src/list.c
    ../../FreeRTOS/Src/timers.c
    ../../FreeRTOS/Src/queue.c
    ../../FreeRTOS/portable/GCC/ARM_CM4F/port.c
    ../../FreeRTOS/portable/MemMang/heap_4.c
)

file(GLOB_RECURSE LVGL_SOURCES
    ../../LVGL/lvgl-9.3.0/*.c
    ../../LVGL/lvgl-9.3.0/src/*.c
    ../../LVGL/lvgl-9.3.0/src/**/*.c
)

target_sources(stm32cubemx INTERFACE
    ${LVGL_SOURCES}
    ${FREERTOS_SOURCES}

    ../../BSP/BSP_User/Src/main.c
    ../../BSP/BSP_User/Src/stm32f4xx_it.c
    ../../BSP/BSP_User/Src/stm32f4xx_it_callback.c
    ../../BSP/BSP_User/Src/stm32f4xx_hal_msp.c
    ../../BSP/BSP_User/Src/system_stm32f4xx.c

    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    # ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c
    ../../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c

    ../../BSP/BSP_User/Src/delay.c
    ../../BSP/BSP_User/Src/myiic.c
    ../../BSP/BSP_User/Src/sys.c
    ../../BSP/BSP_User/Src/syscalls.c
    ../../BSP/BSP_User/Src/sysmem.c

    ../../BSP/BSP_Core/Src/adc.c
    ../../BSP/BSP_Core/Src/dma.c
    ../../BSP/BSP_Core/Src/dma2d.c
    ../../BSP/BSP_Core/Src/fmc.c
    ../../BSP/BSP_Core/Src/i2c.c
    # ../../BSP/BSP_Core/Src/iwdg.c
    ../../BSP/BSP_Core/Src/ltdc.c
    ../../BSP/BSP_Core/Src/tim.c
    # ../../BSP/BSP_Core/Src/wwdg.c
    
    ../../BSP/BSP_Tasks/Src/tasks_sync.c
    ../../BSP/BSP_Tasks/Src/start_task.c
    ../../BSP/BSP_Tasks/Src/led_task.c
    ../../BSP/BSP_Tasks/Src/key_task.c
    ../../BSP/BSP_Tasks/Src/gui_task.c
    ../../BSP/BSP_Tasks/Src/adc_task.c
    ../../BSP/BSP_Tasks/Src/cpu_task.c
    ../../BSP/BSP_Tasks/Src/touch_task.c

    ../../Drivers_Hardware/LED/Src/led.c
    ../../Drivers_Hardware/Key/Src/key.c
    ../../Drivers_Hardware/SDRAM/Src/sdram.c
    # ../../Drivers_Hardware/OLED/Src/oledio.c
    # ../../Drivers_Hardware/OLED/Src/oled.c
    ../../Drivers_Hardware/LCD/Src/lcd.c
    ../../Drivers_Hardware/FT5446/Src/ft5446.c

    ../../APP/Debug_Linker/RTT/Src/SEGGER_RTT.c
    ../../APP/Debug_Linker/RTT/Src/SEGGER_RTT_printf.c
    ../../APP/Debug_Linker/RTT/SEGGER_RTT_ASM_ARMv7M.S
    ../../APP/Multi_Button/Src/multi_button.c
    ../../APP/Multi_Led/Src/multi_led.c
    ../../APP/Algorithm/KalmanFilter/Src/filter.c

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
