################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/fsm/States/subdir.mk
-include source/fsm/subdir.mk
-include source/file_manager/subdir.mk
-include source/fft/subdir.mk
-include source/equalizer/subdir.mk
-include source/audio_manager/subdir.mk
-include source/subdir.mk
-include drivers/SDK/SD/subdir.mk
-include drivers/SDK/subdir.mk
-include drivers/MCAL/subdir.mk
-include drivers/HAL/subdir.mk
-include device/subdir.mk
-include component/lists/subdir.mk
-include component/id3/subdir.mk
-include component/helix/real/subdir.mk
-include component/helix/subdir.mk
-include component/fatfs/fsl_sd_disk/subdir.mk
-include component/fatfs/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SDProject.axf

# Tool invocations
SDProject.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\Facultad\GitMicros\TPFinal\SDProject\libs" -Xlinker -Map="SDProject.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T SDProject_Debug.ld -o "SDProject.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SDProject.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "SDProject.axf"; # arm-none-eabi-objcopy -v -O binary "SDProject.axf" "SDProject.bin" ; # checksum -p MK64FN1M0xxx12 -d "SDProject.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
