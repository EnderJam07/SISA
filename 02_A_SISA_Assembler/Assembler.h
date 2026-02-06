#pragma once

#define ASSEMBLY_STATEMENT_BUFFER_SIZE 100

#define OPCODE_SHIFT 24
#define RD_REG_SHIFT 20
#define RT_REG_SHIFT 20
#define RM_REG_SHIFT 16
#define RN_REG_SHIFT 12

void executeAssembler(char* assemblyFilename, char* machineCodeFilename);

