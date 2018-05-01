# The CMake minimum version
cmake_minimum_required(VERSION 3.5)

# The name of the project
project( myProject )

# Add library for stm32peripherals
add_subdirectory( Libraries )

# Sources for startup code
set ( SOURCES 
    "${CMAKE_SOURCE_DIR}/Device/startup_stm32f0xx.s"
    "${CMAKE_SOURCE_DIR}/src/system_stm32f0xx.c"
    "${CMAKE_SOURCE_DIR}/src/main.c"
)

# The header includes
include_directories( 
    "${CMAKE_SOURCE_DIR}/Libraries"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Device/ST/STM32F0xx/Include"
    "${CMAKE_SOURCE_DIR}/Libraries/STM32F0xx_StdPeriph_Driver/inc"
)
# The additional CMake flags
set (CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -mcpu=cortex-m0 ")
set (CFLAGS  " -Wall -g -std=c99 -Os ")  
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m
set (CFLAGS "${CFLAGS} -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb ")
set (CFLAGS "${CFLAGS} -ffunction-sections -fdata-sections ")
set (CFLAGS "${CFLAGS} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map ")

#set (CFLAGS "${CFLAGS} -I inc -I $(STD_PERIPH_LIB) -I $(STD_PERIPH_LIB)/CMSIS/Device/ST/STM32F0xx/Include")
#set (CFLAGS "${CFLAGS} -I $(STD_PERIPH_LIB)/CMSIS/Include -I $(STD_PERIPH_LIB)/STM32F0xx_StdPeriph_Driver/inc
set (CFLAGS "${CFLAGS} -include ${CMAKE_SOURCE_DIR}/Libraries/stm32f0xx_conf.h ")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  ${CFLAGS}")
# The header includes


set (LINKERPATH     "${CMAKE_SOURCE_DIR}/Device/ldscripts"   ) 

set (CMAKE_EXE_LINKER_FLAGS " -L${LINKERPATH} -Tstm32f0.ld")
#${CMAKE_EXE_LINKER_FLAGS}

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(
    ${PROJECT_NAME}
    stm32drivers
)