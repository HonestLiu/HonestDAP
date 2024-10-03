CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m4)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_C_COMPILER "D:/AppData/RT-ThreadStudio/platform/env_released/env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_ASM_COMPILER "D:/AppData/RT-ThreadStudio/platform/env_released/env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_C_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_ASM_FLAGS " -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2")
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_CXX_COMPILER "D:/AppData/RT-ThreadStudio/platform/env_released/env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-g++.exe")
SET(CMAKE_CXX_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_CXX_COMPILER_WORKS TRUE)

SET(CMAKE_OBJCOPY "D:/AppData/RT-ThreadStudio/platform/env_released/env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy.exe")
SET(CMAKE_SIZE "D:/AppData/RT-ThreadStudio/platform/env_released/env/tools/ConEmu/ConEmu/../../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-size.exe")

SET(CMAKE_EXE_LINKER_FLAGS " -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Wl,--gc-sections,-Map=rt-thread.map,-cref,-u,Reset_Handler -T ${CMAKE_SOURCE_DIR}/board/linker_scripts/link.lds")

SET(CMAKE_CXX_STANDARD 14)
PROJECT(rtthread C CXX ASM)
INCLUDE_DIRECTORIES(
        applications
        packages/CherryUSB-v1.4.0/common
        packages/CherryUSB-v1.4.0/core
        packages/CherryUSB-v1.4.0/class/cdc
        packages/CherryUSB-v1.4.0/class/msc
        packages/CherryUSB-v1.4.0/class/hid
        packages/CherryUSB-v1.4.0/class/audio
        packages/CherryUSB-v1.4.0/class/video
        packages/CherryUSB-v1.4.0/class/wireless
        packages/CherryUSB-v1.4.0/class/dfu
        packages/CherryUSB-v1.4.0/class/midi
        packages/CherryUSB-v1.4.0/class/vendor/net
        packages/CherryUSB-v1.4.0/class/vendor/serial
        packages/CherryUSB-v1.4.0/osal
        rt-thread/components/libc/compilers/common/include
        rt-thread/components/libc/compilers/newlib
        rt-thread/components/drivers/include
        board
        board/CubeMX_Config/Inc
        board/ports
        libraries/HAL_Drivers/drivers
        libraries/HAL_Drivers/drivers/config
        libraries/HAL_Drivers
        libraries/HAL_Drivers/CMSIS/Include
        rt-thread/components/dfs/dfs_v1/include
        rt-thread/components/dfs/dfs_v1/filesystems/devfs
        rt-thread/components/dfs/dfs_v1/filesystems/elmfat
        rt-thread/components/dfs/dfs_v1/filesystems/romfs
        rt-thread/components/finsh
        .
        rt-thread/include
        rt-thread/libcpu/arm/common
        rt-thread/libcpu/arm/cortex-m4
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Inc
        libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Include
        rt-thread/components/libc/posix/io/epoll
        rt-thread/components/libc/posix/io/eventfd
        rt-thread/components/libc/posix/io/poll
        rt-thread/components/libc/posix/ipc
)

ADD_DEFINITIONS(
        -DRT_USING_LIBC
        -DRT_USING_NEWLIBC
        -DSTM32F407xx
        -DUSE_HAL_DRIVER
        -D_POSIX_C_SOURCE=1
        -D__RTTHREAD__
)

SET(PROJECT_SOURCES

        applications/main.c

        packages/CherryUSB-v1.4.0/class/msc/usbd_msc.c
        packages/CherryUSB-v1.4.0/platform/rtthread/usb_check.c
        packages/CherryUSB-v1.4.0/osal/usb_osal_rtthread.c
        packages/CherryUSB-v1.4.0/core/usbd_core.c
        packages/CherryUSB-v1.4.0/platform/rtthread/usb_msh.c
        packages/CherryUSB-v1.4.0/port/dwc2/usb_dc_dwc2.c
        packages/CherryUSB-v1.4.0/class/cdc/usbd_cdc_acm.c
        packages/CherryUSB-v1.4.0/port/dwc2/usb_glue_st.c
        packages/CherryUSB-v1.4.0/class/hid/usbd_hid.c
        rt-thread/components/libc/compilers/common/cctype.c
        rt-thread/components/libc/compilers/common/cstdlib.c
        rt-thread/components/libc/compilers/common/cstring.c
        rt-thread/components/libc/compilers/common/ctime.c
        rt-thread/components/libc/compilers/common/cunistd.c
        rt-thread/components/libc/compilers/common/cwchar.c
        rt-thread/components/libc/compilers/newlib/syscalls.c
        rt-thread/components/drivers/core/device.c
        rt-thread/components/drivers/ipc/completion.c
        rt-thread/components/drivers/ipc/condvar.c
        rt-thread/components/drivers/ipc/dataqueue.c
        rt-thread/components/drivers/ipc/pipe.c
        rt-thread/components/drivers/ipc/ringblk_buf.c
        rt-thread/components/drivers/ipc/ringbuffer.c
        rt-thread/components/drivers/ipc/waitqueue.c
        rt-thread/components/drivers/ipc/workqueue.c
        rt-thread/components/drivers/pin/pin.c
        rt-thread/components/drivers/serial/serial.c
        board/CubeMX_Config/Src/stm32f4xx_hal_msp.c
        board/board.c
        libraries/HAL_Drivers/drivers/drv_gpio.c
        libraries/HAL_Drivers/drivers/drv_usart.c
        libraries/HAL_Drivers/drv_common.c
        libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s
        rt-thread/components/dfs/dfs_v1/filesystems/devfs/devfs.c
        rt-thread/components/dfs/dfs_v1/filesystems/elmfat/dfs_elm.c
        rt-thread/components/dfs/dfs_v1/filesystems/elmfat/ff.c
        rt-thread/components/dfs/dfs_v1/filesystems/elmfat/ffunicode.c
        rt-thread/components/dfs/dfs_v1/filesystems/romfs/dfs_romfs.c
        rt-thread/components/dfs/dfs_v1/src/dfs.c
        rt-thread/components/dfs/dfs_v1/src/dfs_file.c
        rt-thread/components/dfs/dfs_v1/src/dfs_fs.c
        rt-thread/components/dfs/dfs_v1/src/dfs_posix.c
        rt-thread/components/finsh/shell.c
        rt-thread/components/finsh/msh.c
        rt-thread/components/finsh/msh_parse.c
        rt-thread/components/finsh/cmd.c
        rt-thread/components/finsh/msh_file.c
        rt-thread/src/clock.c
        rt-thread/src/components.c
        rt-thread/src/idle.c
        rt-thread/src/ipc.c
        rt-thread/src/irq.c
        rt-thread/src/klibc/kstdio.c
        rt-thread/src/klibc/kstring.c
        rt-thread/src/kservice.c
        rt-thread/src/mem.c
        rt-thread/src/mempool.c
        rt-thread/src/object.c
        rt-thread/src/scheduler_comm.c
        rt-thread/src/scheduler_up.c
        rt-thread/src/thread.c
        rt-thread/src/timer.c
        rt-thread/libcpu/arm/common/atomic_arm.c
        rt-thread/libcpu/arm/common/div0.c
        rt-thread/libcpu/arm/common/showmem.c
        rt-thread/libcpu/arm/cortex-m4/context_gcc.S
        rt-thread/libcpu/arm/cortex-m4/cpuport.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp_ex.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cec.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
        libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c
        libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
        packages/CherryRB/chry_ringbuffer.c
        applications/dap_main.h
        applications/dap_main.c
        applications/usb2uart.c
        applications/dap_loop.c
        board/CubeMX_Config/Drivers/CMSIS/DAP/Firmware/Include/DAP.h
        packages/DAPLink/Source/DAP.c
        packages/DAPLink/Source/SW_DP.c
        packages/DAPLink/Source/SWO.c
        packages/DAPLink/Source/JTAG_DP.c
        packages/DAPLink/Source/swd_host.c
        packages/flmparse/flmparse.c
        packages/DAPLink/Source/SWD_flash.c
        packages/offline_download/swd_download_file.c

)

INCLUDE_DIRECTORIES(packages/DAPLink/Include)
INCLUDE_DIRECTORIES(packages/offline_download)

LINK_DIRECTORIES(
)

LINK_LIBRARIES(
        c
        m
)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES})
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf)