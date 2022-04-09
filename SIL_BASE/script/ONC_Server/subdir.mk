################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ONC_Server/msg_proc.cpp \
../ONC_Server/oncrpc_server.cpp 

O_FILE += \
./ONC_Server/msg_proc.o \
./ONC_Server/oncrpc_server.o 


# Each subdirectory must supply rules for building sources it contributes
ONC_Server/%.o: ../ONC_Server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Platinum C++ compiler'
	arm-linux-gnueabihf-g++.exe -D_PLATINUM -I"${MDS_HOME}crosstool\gcc-linaro-arm-linux-gnu\arm-linux-gnueabihf\libc\usr\include" -I"${MDS_HOME}crosstool\gcc-linaro-arm-linux-gnu\arm-linux-gnueabihf\include\c++\4.7.3" -I"${SIL_WORKSPACE}" -I"${SIL_WORKSPACE}\ONC_Server" -I"${MDS_HOME}Maestro\includes" -I"${MDS_HOME}Maestro\includes\CPP" -I../ -I"${MDS_HOME}Maestro\includes\embl" -I"${MDS_HOME}cygwin\tmp" -O0 -Wall -c -fmessage-length=0 -fPIC -mfpu=vfpv4 -mfloat-abi=hard -mtune=cortex-a15 -Wno-unused -DNO_LOGGING_REALLOC -DDEFAULT_BUFFER_SIZE=65536 -fpermissive -o"$@" "$<"	
	@echo 'Finished building: $<'
	@echo ' '


