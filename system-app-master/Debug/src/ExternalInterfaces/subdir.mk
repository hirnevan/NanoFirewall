################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ExternalInterfaces/GeolocationManager.cpp \
../src/ExternalInterfaces/VirusTotalManager.cpp 

OBJS += \
./src/ExternalInterfaces/GeolocationManager.o \
./src/ExternalInterfaces/VirusTotalManager.o 

CPP_DEPS += \
./src/ExternalInterfaces/GeolocationManager.d \
./src/ExternalInterfaces/VirusTotalManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/ExternalInterfaces/%.o: ../src/ExternalInterfaces/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/usr/local/include" -I"../src" -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ExternalInterfaces/VirusTotalManager.o: ../src/ExternalInterfaces/VirusTotalManager.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/usr/local/include" -I"../src" -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/ExternalInterfaces/VirusTotalManager.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


