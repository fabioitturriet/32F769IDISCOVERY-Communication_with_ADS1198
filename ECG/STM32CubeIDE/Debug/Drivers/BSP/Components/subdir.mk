################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/ProjetoSTM32ECG/ECG/Drivers/BSP/Components/ft6x06/ft6x06.c \
D:/ProjetoSTM32ECG/ECG/Drivers/BSP/Components/otm8009a/otm8009a.c 

C_DEPS += \
./Drivers/BSP/Components/ft6x06.d \
./Drivers/BSP/Components/otm8009a.d 

OBJS += \
./Drivers/BSP/Components/ft6x06.o \
./Drivers/BSP/Components/otm8009a.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ft6x06.o: D:/ProjetoSTM32ECG/ECG/Drivers/BSP/Components/ft6x06/ft6x06.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DDEBUG -c -I../../Core/Inc -I../../Drivers/CMSIS/Include -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../TouchGFX/App -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../TouchGFX/target/generated -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../Drivers/BSP/Components/Common -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../LIBJPEG/App -I../../LIBJPEG/Target -I../../Utilities/JPEG -I../../Middlewares/Third_Party/LibJPEG/include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/generated/videos/include -I../../TouchGFX/gui/include -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_ADS1198" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_ESP01" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/UartRingBuffer" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_panTompkins" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/otm8009a.o: D:/ProjetoSTM32ECG/ECG/Drivers/BSP/Components/otm8009a/otm8009a.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F769xx -DDEBUG -c -I../../Core/Inc -I../../Drivers/CMSIS/Include -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../TouchGFX/App -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../TouchGFX/target/generated -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../Drivers/BSP/Components/Common -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../LIBJPEG/App -I../../LIBJPEG/Target -I../../Utilities/JPEG -I../../Middlewares/Third_Party/LibJPEG/include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/generated/videos/include -I../../TouchGFX/gui/include -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_ADS1198" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_ESP01" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/UartRingBuffer" -I"D:/ProjetoSTM32ECG/ECG/STM32CubeIDE/Application/User/biblioteca_panTompkins" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components

clean-Drivers-2f-BSP-2f-Components:
	-$(RM) ./Drivers/BSP/Components/ft6x06.d ./Drivers/BSP/Components/ft6x06.o ./Drivers/BSP/Components/ft6x06.su ./Drivers/BSP/Components/otm8009a.d ./Drivers/BSP/Components/otm8009a.o ./Drivers/BSP/Components/otm8009a.su

.PHONY: clean-Drivers-2f-BSP-2f-Components

