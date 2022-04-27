#--------------------------------------------------
#
#
#
#--------------------------------------------------


TARGET ?= classA

#GCC_ROOT = /opt/gcc-arm-none-eabi-5_4-2016q3
#CC       = $(GCC_ROOT)/bin/arm-none-eabi-gcc
#CXX      = $(GCC_ROOT)/bin/arm-none-eabi-g++
#LD      = $(GCC_ROOT)/bin/arm-none-eabi-ld
#OBJCOPY  = $(GCC_ROOT)/bin/arm-none-eabi-objcopy
#OBJSIZE  = $(GCC_ROOT)/bin/arm-none-eabi-size

GCC_ROOT = /usr
CC       = $(GCC_ROOT)/bin/arm-linux-gnueabi-gcc
CXX      = $(GCC_ROOT)/bin/arm-linux-gnueabi-g++
LD      = $(GCC_ROOT)/bin/arm-linux-gnueabi-ld
OBJCOPY  = $(GCC_ROOT)/bin/arm-linux-gnueabi-objcopy
OBJSIZE  = $(GCC_ROOT)/bin/arm-linux-gnueabi-size

# ../libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h750xx.s

PLATFORM = RAK811BreakBoard
DEVICE   = STM32H750XBHx
MCU      = stm32
LINKER_DEF = board/linker_scripts/STM32H750XBHx/link.lds
BUILD    = Debug



INCLUDE_DIRS := $(GCC_ROOT)/arm-linux-gnueabi/include


#====================================================
C_SRCS := \
	board/board.c 


#	coIDE/classA/components/coocox-master/Retarget_printf/source/printf.c \
#	src/radio/sx1276/sx1276.c \
#	src/peripherals/lis3dh.c \
#	src/boards/$(PLATFORM)/gps-board.c \
#	src/boards/$(PLATFORM)/rtc-board.c \
#	src/boards/$(PLATFORM)/adc-board.c \
#	src/boards/$(PLATFORM)/cmsis/system_stm32h7xx.c \
#	src/boards/$(PLATFORM)/sx1276-board.c \
#	src/boards/$(PLATFORM)/eeprom-board.c \
#	src/boards/$(PLATFORM)/board.c \
#	src/boards/$(PLATFORM)/i2c-board.c \
#	src/boards/$(PLATFORM)/spi-board.c \
#	src/boards/$(PLATFORM)/gpio-board.c \
#	src/boards/mcu/$(MCU)/sysIrqHandlers.c \
#	src/boards/$(PLATFORM)/uart-board.c \
#	src/system/fifo.c \
#	src/system/timer.c \
#	src/system/delay.c \
#	src/system/gpio.c \
#	src/system/adc.c \
#	src/system/gps.c \
#	src/system/uart.c \
#	src/system/i2c.c \
#	src/system/crypto/cmac.c \
#	src/system/crypto/aes.c \
#	src/mac/LoRaMac.c \
#	src/mac/region/Region.c \
#	src/mac/region/RegionEU868.c \
#	src/mac/region/RegionUS915-Hybrid.c \
#	src/mac/region/RegionAS923.c \
#	src/mac/region/RegionCommon.c \
#	src/mac/region/RegionKR920.c \
#	src/mac/region/RegionAU915.c \
#	src/mac/region/RegionIN865.c \
#	src/mac/region/RegionUS915.c \
#	src/mac/LoRaMacCrypto.c \
#	src/boards/mcu/$(MCU)/utilities.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_rcc.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7_hal_adc.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7_hal_i2c.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_rcc_ex.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_adc_ex.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_dma.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_spi_ex.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_pwr_ex.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_gpio.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_cortex.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_rtc.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_rtc_ex.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_usart.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_uart.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_pwr.c \
#	src/boards/mcu/$(MCU)/$(DEVICE)/Src/stm32h7xx_hal_spi.c 

A_SRCS := \
	../libraries/STM32H7xx_HAL/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h750xx.s

INCLUDE_DIRS += \
	src  \
	src/boards/$(PLATFORM) \
	src/boards/$(PLATFORM)/cmsis  \
	src/boards/mcu/$(MCU)/  \
	src/boards/mcu/$(MCU)/cmsis  \
	src/boards/mcu/$(MCU)/$(DEVICE)/Inc  \
	src/mac  \
	src/mac/region  \
	src/peripherals  \
	src/radio  \
	src/system  \
	src/system/crypto


C_OBJS := ${C_SRCS:.c=.o}
A_OBJS := ${A_SRCS:.s=.o}

OBJS := $(C_OBJS) $(A_OBJS) 

CFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

CFLAGS += -DSTM32H7
CFLAGS += -DUSE_HAL_DRIVER
CFLAGS += -DREGION_EU868
 
ifeq ($(BUILD),Debug)
CFLAGS += -DUSE_DEBUGGER
endif

#for the C
CFLAGS += -mcpu=cortex-m3 -mthumb -g2 -Wall -Os -c
CFLAGS += -MMD -MP -MF"${@:.o=.d}" -MT"$@" -o "$@"
CFLAGS += -fno-builtin-printf


#for the stub
ASFLAGS += -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -x assembler-with-cpp -c 

#for the linker
CXXFLAGS := -mcpu=cortex-m7 -mthumb -gdwarf-2 -Os -g2 -g
CXXFLAGS += -Wl,--gc-sections --specs=nano.specs 
CXXFLAGS += -Xlinker -Map="$(BUILD)/$(TARGET).map"

.PHONY: all clean distclean

all: $(TARGET)
	
%.o: %.c
	$(CC) $(CFLAGS)  "$<"

## Build Stub
%.o: %.s
	mkdir $(BUILD)
	$(CC) $(ASFLAGS) -o "$@" "$<"

## Link
$(TARGET): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C++ Linker'
	$(CC) $(CXXFLAGS)  -T "$(LINKER_DEF)" -o $(BUILD)/$(TARGET).axf $(OBJS) $(S_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '


	@echo 'Building hex file: $(BUILD)/$(TARGET).hex'
	$(OBJCOPY) -O ihex "$(BUILD)/$(TARGET).axf" "$(BUILD)/$(TARGET).hex"
	@echo ' '

	@echo 'Building bin file: $(BUILD)/$(TARGET).bin'
	$(OBJCOPY) -O binary "$(BUILD)/$(TARGET).axf" "$(BUILD)/$(TARGET).bin"
	@echo ' '

	@echo 'Running size tool'
	$(OBJSIZE) "$(BUILD)/$(TARGET).axf"
	@echo ' '

clean:
	@- $(RM) $(OBJS) ${OBJS:.o=.d}
	@- $(RM) -rf $(BUILD)
	@- $(RM) $(TARGET).axf $(TARGET).bin $(TARGET).hex $(TARGET).map

distclean: clean
