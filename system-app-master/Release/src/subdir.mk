################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConfigManager.cpp \
../src/EmailManager.cpp \
../src/EntryPoint.cpp \
../src/EthManager.cpp \
../src/StatisticsManager.cpp \
../src/WebManager.cpp 

OBJS += \
./src/ConfigManager.o \
./src/EmailManager.o \
./src/EntryPoint.o \
./src/EthManager.o \
./src/StatisticsManager.o \
./src/WebManager.o 

CPP_DEPS += \
./src/ConfigManager.d \
./src/EmailManager.d \
./src/EntryPoint.d \
./src/EthManager.d \
./src/StatisticsManager.d \
./src/WebManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/usr/local/include" -I"../src" -I"../include" -O2 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


