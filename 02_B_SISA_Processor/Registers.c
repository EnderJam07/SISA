#include <stdio.h>
#include "Registers.h"

#define REGISTERS_PER_OUTPUT_LINE 4

void RegistersPrintRegisterValues(Registers* registerPtr)
{
	printf("Registers:\n");

	for (int registerIndex = 0; registerIndex < REGISTER_SET_SIZE; registerIndex++)
	{
		printf("%02d: 0x%08X ", registerIndex, registerPtr->values[registerIndex]);

		if ((registerIndex + 1) % REGISTERS_PER_OUTPUT_LINE == 0) printf("\n");
	}

	printf("\n");
}