################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/AdminID.c \
../source/App.c \
../source/CardReader.c \
../source/CardReaderDecoder.c \
../source/CardReaderHAL.c \
../source/EncoderHandler.c \
../source/EventQueue.c \
../source/FSM.c \
../source/FSMTABLE.c \
../source/FSM_actions.c \
../source/SysTick.c \
../source/boardLed.c \
../source/displayLed.c \
../source/displayManager.c \
../source/displaySegment.c \
../source/door.c \
../source/encoder.c \
../source/encoderEvent.c \
../source/encoderHal.c \
../source/fsmState_Menu.c \
../source/gpio.c \
../source/timer.c \
../source/timerqueue.c 

OBJS += \
./source/AdminID.o \
./source/App.o \
./source/CardReader.o \
./source/CardReaderDecoder.o \
./source/CardReaderHAL.o \
./source/EncoderHandler.o \
./source/EventQueue.o \
./source/FSM.o \
./source/FSMTABLE.o \
./source/FSM_actions.o \
./source/SysTick.o \
./source/boardLed.o \
./source/displayLed.o \
./source/displayManager.o \
./source/displaySegment.o \
./source/door.o \
./source/encoder.o \
./source/encoderEvent.o \
./source/encoderHal.o \
./source/fsmState_Menu.o \
./source/gpio.o \
./source/timer.o \
./source/timerqueue.o 

C_DEPS += \
./source/AdminID.d \
./source/App.d \
./source/CardReader.d \
./source/CardReaderDecoder.d \
./source/CardReaderHAL.d \
./source/EncoderHandler.d \
./source/EventQueue.d \
./source/FSM.d \
./source/FSMTABLE.d \
./source/FSM_actions.d \
./source/SysTick.d \
./source/boardLed.d \
./source/displayLed.d \
./source/displayManager.d \
./source/displaySegment.d \
./source/door.d \
./source/encoder.d \
./source/encoderEvent.d \
./source/encoderHal.d \
./source/fsmState_Menu.d \
./source/gpio.d \
./source/timer.d \
./source/timerqueue.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


