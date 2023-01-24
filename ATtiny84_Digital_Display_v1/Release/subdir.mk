################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../7seg.c \
../7seg_new_connection.c \
../POLLING3_Change.c \
../adc.c \
../timer0.c \
../timer1.c 

OBJS += \
./7seg.o \
./7seg_new_connection.o \
./POLLING3_Change.o \
./adc.o \
./timer0.o \
./timer1.o 

C_DEPS += \
./7seg.d \
./7seg_new_connection.d \
./POLLING3_Change.d \
./adc.d \
./timer0.d \
./timer1.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=attiny84 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


