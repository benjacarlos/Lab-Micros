################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/AdminID.c \
../Sources/App.c \
../Sources/FSM.c \
../Sources/FSMTABLE.c \
../Sources/SysTick.c \
../Sources/displayLed.c \
../Sources/displayManager.c \
../Sources/displaySegment.c \
../Sources/encoder.c \
../Sources/encoderEvent.c \
../Sources/encoderHal.c \
../Sources/gpio.c \
../Sources/timer.c 

OBJS += \
./Sources/AdminID.o \
./Sources/App.o \
./Sources/FSM.o \
./Sources/FSMTABLE.o \
./Sources/SysTick.o \
./Sources/displayLed.o \
./Sources/displayManager.o \
./Sources/displaySegment.o \
./Sources/encoder.o \
./Sources/encoderEvent.o \
./Sources/encoderHal.o \
./Sources/gpio.o \
./Sources/timer.o 

C_DEPS += \
./Sources/AdminID.d \
./Sources/App.d \
./Sources/FSM.d \
./Sources/FSMTABLE.d \
./Sources/SysTick.d \
./Sources/displayLed.d \
./Sources/displayManager.d \
./Sources/displaySegment.d \
./Sources/encoder.d \
./Sources/encoderEvent.d \
./Sources/encoderHal.d \
./Sources/gpio.d \
./Sources/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


