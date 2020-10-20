################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/startup/hardware.c \
../SDK/startup/main.c \
../SDK/startup/semihost_hardfault.c \
../SDK/startup/startup_mk64f12.c 

OBJS += \
./SDK/startup/hardware.o \
./SDK/startup/main.o \
./SDK/startup/semihost_hardfault.o \
./SDK/startup/startup_mk64f12.o 

C_DEPS += \
./SDK/startup/hardware.d \
./SDK/startup/main.d \
./SDK/startup/semihost_hardfault.d \
./SDK/startup/startup_mk64f12.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/startup/%.o: ../SDK/startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


