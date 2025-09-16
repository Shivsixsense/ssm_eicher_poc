################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/littlefs/Src/lfs.c \
../Components/littlefs/Src/lfs_util.c \
../Components/littlefs/Src/storage.c \
../Components/littlefs/Src/w25qxx.c \
../Components/littlefs/Src/w25qxx_littlefs.c 

OBJS += \
./Components/littlefs/Src/lfs.o \
./Components/littlefs/Src/lfs_util.o \
./Components/littlefs/Src/storage.o \
./Components/littlefs/Src/w25qxx.o \
./Components/littlefs/Src/w25qxx_littlefs.o 

C_DEPS += \
./Components/littlefs/Src/lfs.d \
./Components/littlefs/Src/lfs_util.d \
./Components/littlefs/Src/storage.d \
./Components/littlefs/Src/w25qxx.d \
./Components/littlefs/Src/w25qxx_littlefs.d 


# Each subdirectory must supply rules for building sources it contributes
Components/littlefs/Src/%.o Components/littlefs/Src/%.su Components/littlefs/Src/%.cyclo: ../Components/littlefs/Src/%.c Components/littlefs/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx '-DCMSIS_device_header=<stm32g0xx.h>' -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0 -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/Application/Inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/can_tp/inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/Eicher/inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/littlefs/Inc" -I"C:/Users/kapil/Downloads/ssm_lora_ can_final 2/ssm_lora_ can_final/Components/LORA/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Components-2f-littlefs-2f-Src

clean-Components-2f-littlefs-2f-Src:
	-$(RM) ./Components/littlefs/Src/lfs.cyclo ./Components/littlefs/Src/lfs.d ./Components/littlefs/Src/lfs.o ./Components/littlefs/Src/lfs.su ./Components/littlefs/Src/lfs_util.cyclo ./Components/littlefs/Src/lfs_util.d ./Components/littlefs/Src/lfs_util.o ./Components/littlefs/Src/lfs_util.su ./Components/littlefs/Src/storage.cyclo ./Components/littlefs/Src/storage.d ./Components/littlefs/Src/storage.o ./Components/littlefs/Src/storage.su ./Components/littlefs/Src/w25qxx.cyclo ./Components/littlefs/Src/w25qxx.d ./Components/littlefs/Src/w25qxx.o ./Components/littlefs/Src/w25qxx.su ./Components/littlefs/Src/w25qxx_littlefs.cyclo ./Components/littlefs/Src/w25qxx_littlefs.d ./Components/littlefs/Src/w25qxx_littlefs.o ./Components/littlefs/Src/w25qxx_littlefs.su

.PHONY: clean-Components-2f-littlefs-2f-Src

