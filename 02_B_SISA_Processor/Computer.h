#ifndef COMPUTER_H
#define COMPUTER_H

/*******************************************************************************
* Module:       Computer
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:  
* Description:  A Computer object contains a CPU object, a MainMemory object,
*               and an OperatingSystem object. 
*******************************************************************************/

#include "CPU.h"
#include "MainMemory.h"
#include "OperatingSystem.h"

typedef struct
{
    CPU cpu;
    MainMemory mainMemory;
    OperatingSystem operatingSystem;

}Computer;

void ComputerInitialize(Computer* computerPtr);
int32_t ComputerExecuteProgram(Computer* computerPtr, char* executableFilename);

#endif