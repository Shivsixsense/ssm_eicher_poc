################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/Eicher/src/eicher.c 

OBJS += \
./Components/Eicher/src/eicher.o 

C_DEPS += \
./Components/Eicher/src/eicher.d 


# Each subdirectory must supply rules for building sources it contributes
Components/Eicher/src/%.o Components/Eicher/src/%.su Components/Eicher/src/%.cyclo: ../Components/Eicher/src/%.c Components/Eicher/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx '-DCMSIS_device_header=<stm32g0xx.h>' -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/Application/Inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/can_tp/inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/Eicher/inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/littlefs/Inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/LORA/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Components-2f-Eicher-2f-src

clean-Components-2f-Eicher-2f-src:
	-$(RM) ./Components/Eicher/src/eicher.cyclo ./Components/Eicher/src/eicher.d ./Components/Eicher/src/eicher.o ./Components/Eicher/src/eicher.su

.PHONY: clean-Components-2f-Eicher-2f-src

