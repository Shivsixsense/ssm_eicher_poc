################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/%.o Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/%.su Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/%.cyclo: ../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/%.c Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx '-DCMSIS_device_header=<stm32g0xx.h>' -c -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/Application/Inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/can_tp/inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/Eicher/inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/sys_utils/inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/System_Card/inc" -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I../USB_Device/App -I../USB_Device/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/flash/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM0

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM0:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.d ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.o ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/port.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM0

