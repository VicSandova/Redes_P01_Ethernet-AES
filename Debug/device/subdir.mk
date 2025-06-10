################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MK66F18.c 

C_DEPS += \
./device/system_MK66F18.d 

OBJS += \
./device/system_MK66F18.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK66FN2M0VMD18 -DPRINTF_ADVANCED_ENABLE=1 -DFRDM_K66F -DFREEDOM -DMCUXPRESSO_SDK -DCPU_MK66FN2M0VMD18_cm4 -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\source" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\mdio" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\phy" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\utilities" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\drivers" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\device" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\component\uart" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\component\lists" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\CMSIS" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\board" -I"D:\Eldic\NXP\MCUXpressoIDE_24.12.148\workspace\P1_enet_txrx_transfer\frdmk66f\driver_examples\enet\txrx_transfer" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_MK66F18.d ./device/system_MK66F18.o

.PHONY: clean-device

