################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/App.c \
../source/CardReader.c \
../source/CardReaderDecoder.c \
../source/CardReaderHAL.c \
../source/SysTick.c \
../source/gpio.c \
../source/timer.c 

OBJS += \
./source/App.o \
./source/CardReader.o \
./source/CardReaderDecoder.o \
./source/CardReaderHAL.o \
./source/SysTick.o \
./source/gpio.o \
./source/timer.o 

C_DEPS += \
./source/App.d \
./source/CardReader.d \
./source/CardReaderDecoder.d \
./source/CardReaderHAL.d \
./source/SysTick.d \
./source/gpio.d \
./source/timer.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


