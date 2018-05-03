# The CMake minimum version
cmake_minimum_required(VERSION 3.5)
# Include utilities
include(/home/p/.cmake/utils.cmake)
# The name of the project
project( myProject ASM C )

# Add library for stm32peripherals
add_subdirectory( Libraries )

# Sources for startup code
set ( SOURCES 
    "${CMAKE_SOURCE_DIR}/Device/startup_stm32f0xx.s"
    "${CMAKE_SOURCE_DIR}/src/system_stm32f0xx.c"
    "${CMAKE_SOURCE_DIR}/src/main.c"
    "${CMAKE_SOURCE_DIR}/src/led.c"
    "${CMAKE_SOURCE_DIR}/src/timer.c"
    "${CMAKE_SOURCE_DIR}/src/usart.c"
    "${CMAKE_SOURCE_DIR}/src/button.c"
    "${CMAKE_SOURCE_DIR}/src/spi.c"
)

# The header includes
include_directories( 
    "${CMAKE_SOURCE_DIR}/inc"
    "${CMAKE_SOURCE_DIR}/Libraries"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Device/ST/STM32F0xx/Include"
    "${CMAKE_SOURCE_DIR}/Libraries/STM32F0xx_StdPeriph_Driver/inc"
)
# The additional CMake flags
set (CFLAGS  " -Wall -g -std=c99 -Os ")  
set (CFLAGS "${CFLAGS} -mlittle-endian -mcpu=cortex-m0  -march=armv6-m -mthumb ")
set (CFLAGS "${CFLAGS} -ffunction-sections -fdata-sections ")
set (CFLAGS "${CFLAGS} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map ")
set (CFLAGS "${CFLAGS} -include ${CMAKE_SOURCE_DIR}/Libraries/stm32f0xx_conf.h ")
set (CMAKE_C_FLAGS "${CFLAGS}")

# Linker path and flags
set (LINKERPATH     "${CMAKE_SOURCE_DIR}/Device/ldscripts"   ) 
set (CMAKE_EXE_LINKER_FLAGS " -LLibraries -L${LINKERPATH} -Tstm32f0.ld")

# Make target here
add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

# Link libraries
target_link_libraries(
    ${PROJECT_NAME}
    stm32drivers
)

# Print Firmware Size
firmware_size(${PROJECT_NAME})


# Generate Object additionally
generate_object(${PROJECT_NAME} .hex ihex)

# Generate Object additionally
generate_object(${PROJECT_NAME} .bin binary)

# Generate Lst
generate_lst(${PROJECT_NAME})

# Configure flasher script for the project
set(BINARY ${PROJECT_NAME}.bin)
set(FLASH_START 0x08000000)

# ST-Link flash command
add_custom_target(flash 
	COMMAND st-flash --reset write ${BINARY} ${FLASH_START}
	DEPENDS ${PROJECT_NAME}
)

# ST-Link erase command
add_custom_target(erase 
	COMMAND st-flash erase
	DEPENDS ${PROJECT_NAME}
)

# ST-Link debugger command
add_custom_target(debug 
	COMMAND ${DEBUGGER} -tui -command ${CMAKE_SOURCE_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} 
	DEPENDS ${PROJECT_NAME}
)

# ST-Link debug server command
add_custom_target(debug-server 
	COMMAND st-util --listen_port 2331
	DEPENDS ${PROJECT_NAME}
)
