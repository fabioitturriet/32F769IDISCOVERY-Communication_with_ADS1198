################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/UartRingBuffer/UartRingbuffer_multi.c 

C_DEPS += \
./Application/User/UartRingBuffer/UartRingbuffer_multi.d 

OBJS += \
./Application/User/UartRingBuffer/UartRingbuffer_multi.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/UartRingBuffer/%.o Application/User/UartRingBuffer/%.su: ../Application/User/UartRingBuffer/%.c Application/User/UartRingBuffer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DDEBUG -c -I../../Core/Inc -I../../Drivers/CMSIS/Include -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../TouchGFX/App -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../TouchGFX/target/generated -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../Drivers/BSP/Components/Common -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Utilities/JPEG -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/generated/videos/include -I../../TouchGFX/gui/include -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_ADS1198" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_ESP01" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/UartRingBuffer" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_panTompkins" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_SDCard" -I../../FATFS/Target -I../../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_Filtros" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-UartRingBuffer

clean-Application-2f-User-2f-UartRingBuffer:
	-$(RM) ./Application/User/UartRingBuffer/UartRingbuffer_multi.d ./Application/User/UartRingBuffer/UartRingbuffer_multi.o ./Application/User/UartRingBuffer/UartRingbuffer_multi.su

.PHONY: clean-Application-2f-User-2f-UartRingBuffer

