/*******************************************************************************
* Module:       Program
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:  
* Description:  This program tests an instruction processor implementation. 
*******************************************************************************/

#include <stdio.h>
#include "Computer.h"

#define USE_COMMAND_LINE_ARGS 0
#define MINIMUM_COMMAND_LINE_ARGS 2

Computer computer;

void main(int argc, char* argv[])
{
	//Local stack allocated pointer so that we can easily access the 
	//Computer's structure members from the debugger's locals window
	Computer* computerPtr = &computer;

	if (USE_COMMAND_LINE_ARGS == 1)
	{
		if (argc < MINIMUM_COMMAND_LINE_ARGS)
		{
			printf("Usage: sisaprocessor MachineCodeFileName.mci [options]\n\n");
			printf("Option: -step 	  Step through execution. Pauses execution after each instruction is processed\n");
			printf("Option: -cusigs   View Main Controller Unit Signals\n");
			printf("Option: -alusigs  View ALU Controller Signals\n");
			printf("Option: -mem 	  View all program memory\n");
			printf("Option: -dmem 	  View the data segment memory\n");
			printf("Option: -cmem 	  View the code segment memory\n");
			printf("\n");
		}
		else
		{
			ComputerInitialize(&computer);

			CPUSetOptions(argc, argv);
			ControllerSetOptions(argc, argv);
			ALUControllerSetOptions(argc, argv);
			MainMemorySetOptions(argc, argv);

			int32_t programReturnValue = ComputerExecuteProgram(computerPtr, argv[1]);
			
			printf("Program return value: %d\n\n", programReturnValue);
		}
	}
	else
	{
		ComputerInitialize(&computer);

		CPUSetOptions(argc, argv);
		ControllerSetOptions(argc, argv);
		ALUControllerSetOptions(argc, argv);
		MainMemorySetOptions(argc, argv);
		
		int32_t programReturnValue = 0;
		
		//programReturnValue = ComputerExecuteProgram(&computer, "01_RegisterReadWrite_Base_10.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "01_RegisterReadWrite_Base_16.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "02_MemoryReadWrite_Base_10.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "02_MemoryReadWrite_Base_16.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "03_Branches_Base_10.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "03_Branches_Base_16.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "04_ArrayTotalLoop_Base_10.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "04_ArrayTotalLoop_Base_16.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "05_StackSegmentReadWrite.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "06_ProcedureCalls.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "07_ProcedureCalls_Saved_Registers.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "08_ProcedureCalls_Data_Segment.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "09_ArrayMax.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "10_ArraySearch.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "11_ArrayTotal_Procedure.mci");
		programReturnValue = ComputerExecuteProgram(&computer, "12_ArraySearch_Procedure.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "13_ArraySearch_Procedure_Stack_Parameters.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "14_ArraySort.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "15_ArraySort_Procedure.mci");
		//programReturnValue = ComputerExecuteProgram(&computer, "16_ArraySort_Procedure_With_Swap.mci");

		printf("Program return value: %d\n\n", programReturnValue);
	}

	printf("Press the enter key to continue...");

	//int c = getchar();
}
