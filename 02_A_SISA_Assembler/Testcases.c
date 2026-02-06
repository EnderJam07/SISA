#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TestCaseController.h"

void Testcases_Execute()
{
	TestAssemblerInstruction("AssemblyCodeInstructions_01_BIN.aci", "AssemblyCodeInstructions_01_BIN.aci",
		"MachineCodeInstructions_01_BIN.mci", "MachineCodeInstructions_01_Valid.mci");

	TestAssemblerInstruction("AssemblyCodeInstructions_01_HEX.aci", "AssemblyCodeInstructions_01_HEX.aci",
		"MachineCodeInstructions_01_HEX.mci", "MachineCodeInstructions_01_Valid.mci");

	TestAssemblerInstruction("AssemblyCodeInstructions_02_BIN.aci", "AssemblyCodeInstructions_02_BIN.aci",
		"MachineCodeInstructions_02_BIN.mci", "MachineCodeInstructions_02_Valid.mci");

	TestAssemblerInstruction("AssemblyCodeInstructions_02_HEX.aci", "AssemblyCodeInstructions_02_HEX.aci",
		"MachineCodeInstructions_02_HEX.mci", "MachineCodeInstructions_02_Valid.mci");

	TestAssemblerInstruction("AssemblyCodeInstructions_03_BIN.aci", "AssemblyCodeInstructions_03_BIN.aci",
		"MachineCodeInstructions_03_BIN.mci", "MachineCodeInstructions_03_Valid.mci");

	TestAssemblerInstruction("AssemblyCodeInstructions_03_HEX.aci", "AssemblyCodeInstructions_03_HEX.aci",
		"MachineCodeInstructions_03_HEX.mci", "MachineCodeInstructions_03_Valid.mci");


	TestAssemblerInstruction("ArrayTotal_BIN.aci", "ArrayTotal_BIN.aci",
		"ArrayTotal_BIN.mci", "ArrayTotal_Valid.mci");

	TestAssemblerInstruction("ArrayTotal_HEX.aci", "ArrayTotal_HEX.aci",
		"ArrayTotal_HEX.mci", "ArrayTotal_Valid.mci");

	TestAssemblerInstruction("ArrayMax_BIN.aci", "ArrayMax_BIN.aci",
		"ArrayMax_BIN.mci", "ArrayMax_Valid.mci");

	TestAssemblerInstruction("ArrayTotal_HEX.aci", "ArrayTotal_HEX.aci",
		"ArrayTotal_HEX.mci", "ArrayTotal_Valid.mci");
}
