# The CMake minimum version
cmake_minimum_required(VERSION 3.5)

# The name of the project
project( stm32drivers )

# The source path
set (STD_PERIPH_SRC_PATH "${CMAKE_SOURCE_DIR}/Libraries/STM32F0xx_StdPeriph_Driver/src/" )

# The additional CMake flags
set (CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -mcpu=cortex-m0 -includestm32f0xx_conf.h ")

# The list of sources part of the library
set (STD_PERIPH_SOURCES
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_adc.c" 
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_cec.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_comp.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_crc.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_dac.c" 
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_dbgmcu.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_dma.c" 
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_exti.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_flash.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_gpio.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_i2c.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_iwdg.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_misc.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_pwr.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_rcc.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_rtc.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_spi.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_syscfg.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_tim.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_usart.c"
    "${STD_PERIPH_SRC_PATH}/stm32f0xx_wwdg.c"
)

# The static library is created as libstm32drivers.a
add_library( ${PROJECT_NAME} STATIC
    ${STD_PERIPH_SOURCES}
)

# The header includes
target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/Libraries"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Device/ST/STM32F0xx/Include"
    "${CMAKE_SOURCE_DIR}/Libraries/STM32F0xx_StdPeriph_Driver/inc"
)