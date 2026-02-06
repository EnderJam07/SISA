#include <stdio.h>
#include "Assembler.h"
#include "Testcases.h"

#define USE_COMMAND_LINE_ARGS 0
#define MINIMUM_COMMAND_LINE_ARGS 3

void main(int argc, char* argv[])
{
	printf("Starting assembler...\n\n");

	if (USE_COMMAND_LINE_ARGS == 1)
	{
		if (argc < MINIMUM_COMMAND_LINE_ARGS)
		{
			printf("Usage: sisaassembler AssemblyCodeFileName.aci MachineCodeFileName.mci\n\n");

			printf("\n");
		}
		else
		{
			executeAssembler(argv[1], argv[2]);
		}
	}
	else
	{
		//Testcases_Execute();
		
		//executeAssembler("01_RegisterReadWrite_Base_10.aci", "01_RegisterReadWrite_Base_10.mci");
		//executeAssembler("01_RegisterReadWrite_Base_16.aci", "01_RegisterReadWrite_Base_16.mci");
		//executeAssembler("02_MemoryReadWrite_Base_10.aci", "02_MemoryReadWrite_Base_10.mci");
		//executeAssembler("02_MemoryReadWrite_Base_16.aci", "02_MemoryReadWrite_Base_16.mci");
		//executeAssembler("03_Branches_Base_10.aci", "03_Branches_Base_10.mci");
		//executeAssembler("03_Branches_Base_16.aci", "03_Branches_Base_16.mci");
		//executeAssembler("04_ArrayTotalLoop_Base_10.aci", "04_ArrayTotalLoop_Base_10.mci");
		//executeAssembler("04_ArrayTotalLoop_Base_16.aci", "04_ArrayTotalLoop_Base_16.mci");
		//executeAssembler("05_StackSegmentReadWrite.aci", "05_StackSegmentReadWrite.mci");
		//executeAssembler("06_ProcedureCalls.aci", "06_ProcedureCalls.mci");
		//executeAssembler("07_ProcedureCalls_Saved_Registers.aci", "07_ProcedureCalls_Saved_Registers.mci");
		//executeAssembler("08_ProcedureCalls_Data_Segment.aci", "08_ProcedureCalls_Data_Segment.mci");
		//executeAssembler("09_ArrayMax.aci", "09_ArrayMax.mci");
		executeAssembler("10_ArraySearch.aci", "10_ArraySearch.mci");
		//executeAssembler("11_ArrayTotal_Procedure.aci", "11_ArrayTotal_Procedure.mci");
		//executeAssembler("12_ArraySearch_Procedure.aci", "12_ArraySearch_Procedure.mci");
		//executeAssembler("13_ArraySearch_Procedure_Stack_Parameters.aci", "13_ArraySearch_Procedure_Stack_Parameters.mci");
		//executeAssembler("14_ArraySort.aci", "14_ArraySort.mci");
		//executeAssembler("15_ArraySort_Procedure.aci", "15_ArraySort_Procedure.mci");
		//executeAssembler("16_ArraySort_Procedure_With_Swap.aci", "16_ArraySort_Procedure_With_Swap.mci");
	}	

    printf("Press the enter key to continue...");

	//int c = getchar();
}