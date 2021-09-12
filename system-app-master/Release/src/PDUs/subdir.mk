################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PDUs/tls.cpp 

OBJS += \
./src/PDUs/tls.o 

CPP_DEPS += \
./src/PDUs/tls.d 


# Each subdirectory must supply rules for building sources it contributes
src/PDUs/%.o: ../src/PDUs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/usr/local/include" -I"../src" -I"../include" -O2 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


