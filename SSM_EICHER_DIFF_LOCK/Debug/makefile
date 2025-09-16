################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_Device/Target/subdir.mk
-include USB_Device/App/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Drivers/STM32G0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Components/sys_utils/src/subdir.mk
-include Components/flash/src/subdir.mk
-include Components/can_tp/src/subdir.mk
-include Components/System_Card/src/subdir.mk
-include Components/Eicher/src/subdir.mk
-include Components/Application/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := SSM_EICHER_DIFF_LOCK
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
SSM_EICHER_DIFF_LOCK.elf \

MAP_FILES += \
SSM_EICHER_DIFF_LOCK.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
SSM_EICHER_DIFF_LOCK.list \

OBJCOPY_HEX += \
SSM_EICHER_DIFF_LOCK.hex \

OBJCOPY_SREC += \
SSM_EICHER_DIFF_LOCK.srec \


# All Target
all: main-build

# Main-build Target
main-build: SSM_EICHER_DIFF_LOCK.elf secondary-outputs

# Tool invocations
SSM_EICHER_DIFF_LOCK.elf SSM_EICHER_DIFF_LOCK.map: $(OBJS) $(USER_OBJS) C:\Users\kapil\Downloads\SSM_EICHER_DIFF_LOCK\ Clutch\ and\ brake\ bug\ fix\ and\ iad\ and\ iwd\ time\ fixed\ (1)\SSM_EICHER_DIFF_LOCK\ (2)\SSM_EICHER_DIFF_LOCK\STM32G0B1CETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "SSM_EICHER_DIFF_LOCK.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\kapil\Downloads\SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)\SSM_EICHER_DIFF_LOCK (2)\SSM_EICHER_DIFF_LOCK\STM32G0B1CETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="SSM_EICHER_DIFF_LOCK.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

SSM_EICHER_DIFF_LOCK.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "SSM_EICHER_DIFF_LOCK.list"
	@echo 'Finished building: $@'
	@echo ' '

SSM_EICHER_DIFF_LOCK.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "SSM_EICHER_DIFF_LOCK.hex"
	@echo 'Finished building: $@'
	@echo ' '

SSM_EICHER_DIFF_LOCK.srec: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O srec $(EXECUTABLES) "SSM_EICHER_DIFF_LOCK.srec"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) SSM_EICHER_DIFF_LOCK.elf SSM_EICHER_DIFF_LOCK.hex SSM_EICHER_DIFF_LOCK.list SSM_EICHER_DIFF_LOCK.map SSM_EICHER_DIFF_LOCK.srec default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_SREC)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
