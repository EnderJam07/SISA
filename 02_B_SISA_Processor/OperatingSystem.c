#include "OperatingSystem.h"
#include "MainMemory.h"
#include <stdio.h>
#include <stdlib.h>

//PRIVATE FUNCTION PROTOTYPES
int32_t OperatingSystemLoadExecutableFile(OperatingSystem* operatingSystemPtr, char* executableFilename, 
    uint32_t* codeSegmentMemoryAddress, uint32_t* dataSegmentMemoryAddress, uint32_t* programSize);

#define PROGRAM_START_ADDRESS 0x00000000

void OperatingSystemInitialize(OperatingSystem* operatingSystemPtr, CPU* cpuPtr, MainMemory*  mainMemoryPtr)
{
    operatingSystemPtr->cpuPtr = cpuPtr;
    operatingSystemPtr->mainMemoryPtr = mainMemoryPtr;
}

int32_t OperatingSystemExecuteProgram(OperatingSystem* operatingSystemPtr, char* executableFilename)
{
    int32_t returnValue = -1;

    uint32_t programSize = 0;

    uint32_t codeSegmentMemoryAddress = 0;
    uint32_t dataSegmentMemoryAddress = 0;
    
    returnValue = OperatingSystemLoadExecutableFile(operatingSystemPtr, executableFilename, &codeSegmentMemoryAddress, 
        &dataSegmentMemoryAddress, &programSize);
    
    if (returnValue != -1)
    {
        returnValue = CPUExecuteProgram(operatingSystemPtr->cpuPtr, codeSegmentMemoryAddress, dataSegmentMemoryAddress, programSize, MAIN_MEMORY_SIZE);
    }

    return returnValue;
}

int32_t OperatingSystemLoadExecutableFile(OperatingSystem* operatingSystemPtr, char* executableFilename, 
    uint32_t* codeSegmentMemoryAddress, uint32_t* dataSegmentMemoryAddress, uint32_t* programSize)
{
    //Open executable file
    FILE* executableFilenameFilePtr = fopen(executableFilename, "rb");

    if (executableFilenameFilePtr == NULL)
    {
        printf("Executable file does not exist: %s\n\n", executableFilename);

        return -1;
    }

    //Read size of data segment
    uint32_t dataSegmentSize = 0;
    size_t result = fread(&dataSegmentSize, sizeof(uint32_t), 1, executableFilenameFilePtr);

    //Read size of code segment
    uint32_t codeSegmentSize = 0;
    result = fread(&codeSegmentSize, sizeof(uint32_t), 1, executableFilenameFilePtr);

    *codeSegmentMemoryAddress = PROGRAM_START_ADDRESS;
    *dataSegmentMemoryAddress = PROGRAM_START_ADDRESS + codeSegmentSize;

    //Load data segment

    uint32_t mainMemoryAddress = *dataSegmentMemoryAddress;

    for (uint32_t index = 0; index < dataSegmentSize; index++)
    {
        uint32_t dataSegmentWord = 0;
        result = fread(&dataSegmentWord, sizeof(uint32_t), 1, executableFilenameFilePtr);

        operatingSystemPtr->mainMemoryPtr->values[mainMemoryAddress] = dataSegmentWord;

        mainMemoryAddress++;
    }

    //Load code segment

    mainMemoryAddress = *codeSegmentMemoryAddress;

    for (uint32_t index = 0; index < codeSegmentSize; index++)
    {
        uint32_t instruction = 0;
        result = fread(&instruction, sizeof(uint32_t), 1, executableFilenameFilePtr);

        operatingSystemPtr->mainMemoryPtr->values[mainMemoryAddress] = instruction;

        mainMemoryAddress++;
    }


    fclose(executableFilenameFilePtr);

    *programSize = dataSegmentSize + codeSegmentSize;

    return 0;
}