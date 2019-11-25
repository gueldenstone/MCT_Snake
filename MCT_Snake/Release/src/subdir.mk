################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/config.c \
../src/interrupts.c \
../src/main.c \
../src/system_stm32f3xx.c 

OBJS += \
./src/config.o \
./src/interrupts.o \
./src/main.o \
./src/system_stm32f3xx.o 

C_DEPS += \
./src/config.d \
./src/interrupts.d \
./src/main.d \
./src/system_stm32f3xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F30 -DNUCLEO_F303RE -DSTM32F3 -DSTM32F303RETx -DSTM32 -DUSE_HAL_DRIVER -DSTM32F303xE -I"/Users/lukas/Library/Mobile Documents/com~apple~CloudDocs/05_DEV/git/MCT/MCT_Snake/inc" -I"/Users/lukas/Library/Mobile Documents/com~apple~CloudDocs/05_DEV/git/MCT/MCT_Snake/CMSIS/core" -I"/Users/lukas/Library/Mobile Documents/com~apple~CloudDocs/05_DEV/git/MCT/MCT_Snake/CMSIS/device" -I"/Users/lukas/Library/Mobile Documents/com~apple~CloudDocs/05_DEV/git/MCT/MCT_Snake/HAL_Driver/Inc/Legacy" -I"/Users/lukas/Library/Mobile Documents/com~apple~CloudDocs/05_DEV/git/MCT/MCT_Snake/HAL_Driver/Inc" -I"/Users/lukas/Library/Mobile Documents/com~apple~CloudDocs/05_DEV/git/MCT/MCT_Snake/Utilities/STM32F3xx-Nucleo" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


