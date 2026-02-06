#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H

/*******************************************************************************
* Module:       OperatingSystem
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:
* Description:  
*******************************************************************************/

#include "CPU.h"
#include "MainMemory.h"


#include <stdint.h>

typedef struct
{
	CPU* cpuPtr;
	MainMemory* mainMemoryPtr;

}OperatingSystem;

void OperatingSystemInitialize(OperatingSystem* operatingSystemPtr, CPU* cpuPtr, MainMemory*  mainMemoryPtr);
int32_t OperatingSystemExecuteProgram(OperatingSystem* operatingSystemPtr, char* executableFilename);

#endif

