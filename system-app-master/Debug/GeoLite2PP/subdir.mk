################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GeoLite2PP/GeoLite2PP.cpp \
../GeoLite2PP/GeoLite2PP_error_category.cpp 

OBJS += \
./GeoLite2PP/GeoLite2PP.o \
./GeoLite2PP/GeoLite2PP_error_category.o 

CPP_DEPS += \
./GeoLite2PP/GeoLite2PP.d \
./GeoLite2PP/GeoLite2PP_error_category.d 


# Each subdirectory must supply rules for building sources it contributes
GeoLite2PP/%.o: ../GeoLite2PP/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/usr/local/include" -I"../src" -I"../include" -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


