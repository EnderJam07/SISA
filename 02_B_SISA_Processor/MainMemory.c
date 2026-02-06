#include <stdio.h>
#include<string.h>
#include "MainMemory.h"

#define PRINT_MEMORY_VALUES 1
#define PRINT_DATA_SEGMENT_MEMORY_VALUES 1
#define PRINT_CODE_SEGMENT_MEMORY_VALUES 1
#define PRINT_WORD_BASED_MEMORY_ADDESSES 1
#define PRINT_WORD_BASED_DATA_SEGMENT_MEMORY_ADDESSES 0
#define PRINT_WORD_BASED_CODE_SEGMENT_MEMORY_ADDESSES 0
#define	WORDS_PER_OUTPUT_LINE 4

//MODULE LEVEL VARIABLES
int printMemoryValues = 0;
int printDataSegmentMemoryValues = 0;
int printCodeSegmentMemoryValues = 0;

void MainMemoryInitialize(MainMemory* mainMemoryPtr)
{
    for (uint32_t index = 0; index < MAIN_MEMORY_SIZE; index++)
    {
        mainMemoryPtr->values[index] = 0;
    }
}

void MainMemoryPrintValues(MainMemory* mainMemoryPtr, uint32_t startAddress, uint32_t endAddress)
{
	if (printMemoryValues == PRINT_MEMORY_VALUES)
	{
		uint32_t memoryValueCounter = 0;

		printf("%s:\n", "Program Memory");

		for (uint32_t address = startAddress; address < endAddress; address++)
		{
			if (PRINT_WORD_BASED_MEMORY_ADDESSES == 1)
			{
				printf("0x%08X: 0x%08X ", address, mainMemoryPtr->values[address]);

				if ((memoryValueCounter + 1) % WORDS_PER_OUTPUT_LINE == 0) printf("\n");

				memoryValueCounter++;
			}
			else
			{
				uint32_t byteAdjustedAddress = address << 2;

				printf("0x%08X: 0x%08X ", byteAdjustedAddress, mainMemoryPtr->values[address]);

				if ((memoryValueCounter + 1) % WORDS_PER_OUTPUT_LINE == 0) printf("\n");

				memoryValueCounter++;
			}
		}

		printf("\n");
	}
}

void MainMemoryPrintDataSegmentValues(MainMemory* mainMemoryPtr, uint32_t startAddress, uint32_t endAddress)
{
	if (printDataSegmentMemoryValues == PRINT_DATA_SEGMENT_MEMORY_VALUES)
	{
		printf("Data Segment Memory:\n");

		for (uint32_t address = startAddress; address < endAddress; address++)
		{
			if (PRINT_WORD_BASED_DATA_SEGMENT_MEMORY_ADDESSES == 1)
			{
				printf("0x%08X: 0x%08X\n", address, mainMemoryPtr->values[address]);
			}
			else
			{
				uint32_t byteAdjustedAddress = address << 2;

				printf("0x%08X: 0x%08X\n", byteAdjustedAddress, mainMemoryPtr->values[address]);
			}
		}

		printf("\n\n");
	}
}

void MainMemoryPrintCodeSegmentValues(MainMemory* mainMemoryPtr, uint32_t startAddress, uint32_t endAddress)
{
	if (printCodeSegmentMemoryValues == PRINT_CODE_SEGMENT_MEMORY_VALUES)
	{
		printf("Code Segment Memory:\n");

		for (uint32_t address = startAddress; address < endAddress; address++)
		{
			if (PRINT_WORD_BASED_CODE_SEGMENT_MEMORY_ADDESSES == 1)
			{
				printf("0x%08X: 0x%08X\n", address, mainMemoryPtr->values[address]);
			}
			else
			{
				uint32_t byteAdjustedAddress = address << 2;

				printf("0x%08X: 0x%08X\n", byteAdjustedAddress, mainMemoryPtr->values[address]);
			}
		}

		printf("\n");
	}
}

void MainMemorySetOptions(int argc, char* argv[])
{
	for (int commandLineParameter = 1; commandLineParameter < argc; commandLineParameter++)
	{
		int result = strcmp(argv[commandLineParameter], "-mem");

		if (result == 0)
		{
			printMemoryValues = PRINT_MEMORY_VALUES;
			printDataSegmentMemoryValues = PRINT_DATA_SEGMENT_MEMORY_VALUES;
			printCodeSegmentMemoryValues = PRINT_CODE_SEGMENT_MEMORY_VALUES;
		}

		result = strcmp(argv[commandLineParameter], "-dmem");

		if (result == 0) printDataSegmentMemoryValues = PRINT_DATA_SEGMENT_MEMORY_VALUES;

		result = strcmp(argv[commandLineParameter], "-cmem");

		if (result == 0) printCodeSegmentMemoryValues = PRINT_CODE_SEGMENT_MEMORY_VALUES;
	}
}