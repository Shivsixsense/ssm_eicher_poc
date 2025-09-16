################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/System_Card/src/card.c \
../Components/System_Card/src/card_uart_interface.c \
../Components/System_Card/src/com_interface.c 

OBJS += \
./Components/System_Card/src/card.o \
./Components/System_Card/src/card_uart_interface.o \
./Components/System_Card/src/com_interface.o 

C_DEPS += \
./Components/System_Card/src/card.d \
./Components/System_Card/src/card_uart_interface.d \
./Components/System_Card/src/com_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Components/System_Card/src/%.o Components/System_Card/src/%.su Components/System_Card/src/%.cyclo: ../Components/System_Card/src/%.c Components/System_Card/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx '-DCMSIS_device_header=<stm32g0xx.h>' -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/Eicher/inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/can_tp/inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/Application/Inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/sys_utils/inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/System_Card/inc" -I"C:/Users/kapil/Downloads/SSM_EICHER_DIFF_LOCK Clutch and brake bug fix and iad and iwd time fixed (1)/SSM_EICHER_DIFF_LOCK (2)/SSM_EICHER_DIFF_LOCK/Components/flash/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Components-2f-System_Card-2f-src

clean-Components-2f-System_Card-2f-src:
	-$(RM) ./Components/System_Card/src/card.cyclo ./Components/System_Card/src/card.d ./Components/System_Card/src/card.o ./Components/System_Card/src/card.su ./Components/System_Card/src/card_uart_interface.cyclo ./Components/System_Card/src/card_uart_interface.d ./Components/System_Card/src/card_uart_interface.o ./Components/System_Card/src/card_uart_interface.su ./Components/System_Card/src/com_interface.cyclo ./Components/System_Card/src/com_interface.d ./Components/System_Card/src/com_interface.o ./Components/System_Card/src/com_interface.su

.PHONY: clean-Components-2f-System_Card-2f-src

