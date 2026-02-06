#include "MMU.h"

void MMUInitialize(MMU* mmuPtr, MainMemory* mainMemoryPtr)
{
    mmuPtr->mainMemoryPtr = mainMemoryPtr;
}

int16_t MMULoadWord(MMU* mmuPtr, int16_t memoryAddress)
{
    int16_t word = 0;

    word = mmuPtr->mainMemoryPtr->values[memoryAddress];

    return word;
}
