################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf ../*.launch

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Simulink/subdir.mk
-include ONC_Server/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: pre-build main-build

# Main-build Target
main-build: SIL_BASE.pexe

# Tool invocations
SIL_BASE.pexe: $(O_FILE)
	@echo 'Building target: $@'
	@echo 'Invoking: Platinum C++ linker'
	arm-linux-gnueabihf-g++.exe -o"SIL_BASE.pexe" $(O_FILE) -L"${MDS_HOME}Maestro/lib/platinum/" -L"${MDS_HOME}crosstool/gcc-linaro-arm-linux-gnu/arm-linux-gnueabihf/libc/usr/lib/arm-linux-gnueabihf" -L"${MDS_HOME}crosstool/gcc-linaro-arm-linux-gnu/arm-linux-gnueabihf/lib" -L"${MDS_HOME}crosstool/gcc-linaro-arm-linux-gnu/arm-linux-gnueabihf/lib/arm-linux-gnueabi" -Wl,-Bstatic -lMMCPPLIB -lMMC_APP_LIB -Wl,-Bdynamic -lpthread -lrt -mfpu=vfpv4 -mfloat-abi=hard -fpermissive -DDEFAULT_BUFFER_SIZE=50 -mtune=cortex-a15
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build 

# Other Targets
clean:
	-$(RM) $(O_FILE)$(EXE_FILE) SIL_BASE.pexe
	-@echo ' '

pre-build:
	-@echo ' '

post-build:
	-"${MDS_HOME}ElmoUtils/scripts/generateLaunchConfiguration.bat" "SIL_BASE" "${SIL_WORKSPACE}" "PLATINUM_R"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
