#pragma once

/*******************************************************************************
* Module:       MMU
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:  
* Description:  
*******************************************************************************/

#include <stdbool.h>
#include "MainMemory.h"

typedef struct
{
    MainMemory* mainMemoryPtr;

}MMU;

void MMUInitialize(MMU* mmuPtr, MainMemory* mainMemoryPtr);
int16_t MMULoadWord(MMU* mmuPtr, int16_t memoryAddress);