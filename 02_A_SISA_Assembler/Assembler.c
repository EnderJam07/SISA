#pragma warning(disable : 4996)
//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "Assembler.h"
#include "RegisterSet.h"
#include "InstructionOpcodes.h"

//Private Functions
int32_t assemble(char* assemblyCodeFilename, char* machineCodeFilename);
void generate_ALU_RegisterLoadImmediate_MachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void generate_ALU_RegisterImmediate_MachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void generate_ALU_AllRegisterInput_MachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void generateMemoryReadOrWriteMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void generateConditionalBranchMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void generateUnconditionalBranchMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void generateBranchAndLinkMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void generateBranchLinkRegisterMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction);
void writeMachineCodeInstructionToFile(FILE* fptr, uint32_t machineCodeInstruction);
void writeDssSize(FILE* fptr, char* dssSize);
void processDataWordDeclaration(FILE* fptrWrite, char* token, uint32_t* dataWordCount);
void writeDataWord(FILE* fptr, char* dataWord, uint32_t* dataWordCount);
void writeDataWordArray(FILE* fptrWrite, char* token, char* arrayValues, uint32_t* dataWordCount);
void appendOpcode(char* mnemonic, uint32_t* machineInstruction, uint32_t leftShift);
void appendRegisterNumber(char* registerNumber, uint32_t* machineInstruction, uint32_t leftShift);
void append16BitImmediateValue(char* immediateValue, uint32_t* machineInstruction);
void createMachineCodeFile(const char* const codeSegmentFilename, const char* const dataSegmentFilename, char* machineCodeFilename, uint32_t dataWordCount, uint32_t instructionCount);

const char* const DATA_SEGMENT_FILENAME = "DataSegmentFilename.bin";
const char* const CODE_SEGMENT_FILENAME = "CodeSegmentFilename.bin";

void executeAssembler(char* assemblyCodeFilename, char* machineCodeFilename)
{
    printf("************************BEGIN ASSEMBLY PROCESS************************\n\n");

    printf("Assembly code file: %s\n", assemblyCodeFilename);
    printf("Machine code file:  %s\n\n", machineCodeFilename);

    printf("Processing assembly code file...\n\n");

    int32_t returnValue = assemble(assemblyCodeFilename, machineCodeFilename);

    if (returnValue == -1) return;

    printf("\n");

    printf("Assembly processing complete...\n\n");

    printf("Generated machine code...\n\n");

    uint32_t machineCodeFileLine = 0;

    FILE* fptrGeneratedObjectCode = fopen(machineCodeFilename, "rb");
    
    size_t result = fread(&machineCodeFileLine, sizeof(uint32_t), 1, fptrGeneratedObjectCode);

    while (result != 0)
    {
        
        printf("0x%08x\n", machineCodeFileLine);

        result = fread(&machineCodeFileLine, sizeof(uint32_t), 1, fptrGeneratedObjectCode);
    }

    printf("\n");

    printf("*************************END ASSEMBLY PROCESS*************************\n\n");
}

int32_t assemble(char* assemblyCodeFilename, char* machineCodeFilename)
{
    bool tokenIsValid = false;

    uint32_t dataWordCount = 0;
    uint32_t instructionCount = 0;

    char fileline[ASSEMBLY_STATEMENT_BUFFER_SIZE] = { 0 };

    FILE* fptrAssemblyCodeFile = fopen(assemblyCodeFilename, "r");
    FILE* fptrDataSegmentFile = fopen(DATA_SEGMENT_FILENAME, "wb");
    FILE* fptrCodeSegmentFile = fopen(CODE_SEGMENT_FILENAME, "wb");

    char* result = fgets(fileline, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptrAssemblyCodeFile);

    while (result != 0)
    {
        uint32_t machineCodeInstruction = 0;
        int cmpResult = 0;

        fileline[strcspn(fileline, "\r")] = 0; //Linux
        fileline[strcspn(fileline, "\n")] = 0; //Windows

        printf("%s\n", fileline);

        int length = strlen(fileline); //Check for empty lines

        if (length != 0 && fileline[0] != '#')
        {
            char* token = strtok(fileline, " \t");

            cmpResult = strcmp(token, DATA_SEGMENT_START);

            if (cmpResult == 0)
            {
                dataWordCount = 0;

                tokenIsValid = true;
            }

            cmpResult = strcmp(token, CODE_SEGMENT_START);

            if (cmpResult == 0)
            {
                instructionCount = 0;

                tokenIsValid = true;
            }

            cmpResult = strcmp(token, WORD);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                processDataWordDeclaration(fptrDataSegmentFile, token, &dataWordCount);
            }

            cmpResult = strcmp(token, LDWI);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //LDWI R1, 1
                //LDWI R1, h1

                uint32_t machineCodeInstruction = 0;

                generate_ALU_RegisterLoadImmediate_MachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }
            
            cmpResult = strcmp(token, ADDI);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //ADDI R1, R0, 1
                //ADDI R1, R0, h1

                uint32_t machineCodeInstruction = 0;

                generate_ALU_RegisterImmediate_MachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, ADD);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //ADD R2, R0, R1

                uint32_t machineCodeInstruction = 0;

                generate_ALU_AllRegisterInput_MachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, SUB);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //SUB R2, R0, R1

                uint32_t machineCodeInstruction = 0;

                generate_ALU_AllRegisterInput_MachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, MUL);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //MUL R2, R0, R1

                uint32_t machineCodeInstruction = 0;

                generate_ALU_AllRegisterInput_MachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, DIV);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //DIV R2, R0, R1

                uint32_t machineCodeInstruction = 0;

                generate_ALU_AllRegisterInput_MachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, LDW);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //LDW R3, [R0 + 0]   (Optional offset in decimal)
                //LDW R3, [R0 + h0]  (Optional offset in hexidecimal)
                //LDW R3, [R0]       (No offset)

                uint32_t machineCodeInstruction = 0;

                generateMemoryReadOrWriteMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, STW);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //STW R3, [R0 + 0]   (Optional offset in decimal)
                //STW R3, [R0 + h0]  (Optional offset in hexidecimal)
                //STW R3, [R0]       (No offset)

                uint32_t machineCodeInstruction = 0;

                generateMemoryReadOrWriteMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, BE);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //BE R3, 4
                //BE R3, h4

                uint32_t machineCodeInstruction = 0;

                generateConditionalBranchMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, BNE);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                uint32_t machineCodeInstruction = 0;

                generateConditionalBranchMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, BL);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                uint32_t machineCodeInstruction = 0;

                generateConditionalBranchMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, BG);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                uint32_t machineCodeInstruction = 0;

                generateConditionalBranchMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, BLE);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                uint32_t machineCodeInstruction = 0;

                generateConditionalBranchMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, BGE);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                uint32_t machineCodeInstruction = 0;

                generateConditionalBranchMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, B);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //B 4
                //B h4

                uint32_t machineCodeInstruction = 0;

                generateUnconditionalBranchMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, JAL);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //JAL 20
                //JAL h14 

                uint32_t machineCodeInstruction = 0;

                generateBranchAndLinkMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, JLR);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                //JLR 

                uint32_t machineCodeInstruction = 0;

                generateBranchLinkRegisterMachineCodeInstruction(token, &machineCodeInstruction);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }

            cmpResult = strcmp(token, HALT);

            if (cmpResult == 0)
            {
                tokenIsValid = true;

                appendOpcode(token, &machineCodeInstruction, OPCODE_SHIFT);

                writeMachineCodeInstructionToFile(fptrCodeSegmentFile, machineCodeInstruction);

                instructionCount++;
            }
        }
        else tokenIsValid = true;

        if (tokenIsValid == false)
        {
            printf("\nIllegal assembly mnemonic: %s\n\n", fileline);

            return -1;
        }

        tokenIsValid = false; //reset to false

        result = fgets(fileline, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptrAssemblyCodeFile);
    }

    fclose(fptrAssemblyCodeFile);
    fclose(fptrDataSegmentFile);
    fclose(fptrCodeSegmentFile);

    createMachineCodeFile(CODE_SEGMENT_FILENAME, DATA_SEGMENT_FILENAME, machineCodeFilename, dataWordCount, instructionCount);

    return 0;
}

void writeMachineCodeInstructionToFile(FILE* fptrCodeSegmentFile, uint32_t machineInstruction)
{
    size_t result = fwrite(&machineInstruction, sizeof(uint32_t), 1, fptrCodeSegmentFile);

    //result = fwrite(&machineInstruction, sizeof(uint32_t), 1, fptr);
}

void generate_ALU_RegisterLoadImmediate_MachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    char* destReg = strtok(NULL, ",");
    char* immediate = strtok(NULL, " \t#\0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(destReg, machineCodeInstruction, RD_REG_SHIFT);
    append16BitImmediateValue(immediate, machineCodeInstruction);
}

void generate_ALU_RegisterImmediate_MachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    char* destReg = strtok(NULL, ",");
    char* src1Reg = strtok(NULL, ", ");
    char* immediate = strtok(NULL, " \t#\0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(destReg, machineCodeInstruction, RD_REG_SHIFT);
    appendRegisterNumber(src1Reg, machineCodeInstruction, RM_REG_SHIFT);
    append16BitImmediateValue(immediate, machineCodeInstruction);
}

void generate_ALU_AllRegisterInput_MachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    char* destReg = strtok(NULL, ", ");
    char* src1Reg = strtok(NULL, ", ");
    char* src2Reg = strtok(NULL, " \t#\0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(destReg, machineCodeInstruction, RD_REG_SHIFT);
    appendRegisterNumber(src1Reg, machineCodeInstruction, RM_REG_SHIFT);
    appendRegisterNumber(src2Reg, machineCodeInstruction, RN_REG_SHIFT);
}

void generateMemoryReadOrWriteMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    //LDW R1, [R0 + h0]	#Load x into R1
    //LDW R1, [R0+h0]	#Load x into R1
    //LDW R1, [R0]	#Load x into R1

    char* destOrSrcReg = strtok(NULL, ",");
    char* bracketRemoval = strtok(NULL, "[");
    char* bracketContent = strtok(NULL, "]");

    char* memoryAddressBaseReg = NULL;
    char* memoryAddressOffset = NULL;

    char* isPlus = strchr(bracketContent, '+');

    if (isPlus != NULL)
    {
        memoryAddressBaseReg = strtok(bracketContent, " +\0");
        memoryAddressOffset = strtok(NULL, " +\0");
    }
    else
    {
        memoryAddressBaseReg = strtok(bracketContent, "\0");
        memoryAddressOffset = "0";
    }
    
    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(destOrSrcReg, machineCodeInstruction, RT_REG_SHIFT);
    appendRegisterNumber(memoryAddressBaseReg, machineCodeInstruction, RM_REG_SHIFT);
    append16BitImmediateValue(memoryAddressOffset, machineCodeInstruction);

}

void generateConditionalBranchMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    char* src1Reg = strtok(NULL, ", ");
    char* src2Reg = strtok(NULL, ", ");
    char* memoryAddressOffset = strtok(NULL, " \t#\0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(src1Reg, machineCodeInstruction, RD_REG_SHIFT);
    appendRegisterNumber(src2Reg, machineCodeInstruction, RM_REG_SHIFT);
    append16BitImmediateValue(memoryAddressOffset, machineCodeInstruction);
}

void generateUnconditionalBranchMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    signed char* pcOffset = strtok(NULL, " \t#\0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    append16BitImmediateValue(pcOffset, machineCodeInstruction);
}

void generateBranchAndLinkMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    signed char* procedureAbsoluteAddress = strtok(NULL, " \t#\0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    append16BitImmediateValue(procedureAbsoluteAddress, machineCodeInstruction);
}

void generateBranchLinkRegisterMachineCodeInstruction(char* token, uint32_t* machineCodeInstruction)
{
    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
}

void appendOpcode(char* mnemonic, uint32_t* machineInstruction, uint32_t leftShift)
{
    uint32_t machineOpcode = 0;
    int cmpResult = 0;

    cmpResult = strcmp(mnemonic, LDWI);
    if (cmpResult == 0) machineOpcode = OPCODE_LDWI;

    cmpResult = strcmp(mnemonic, ADDI);
    if (cmpResult == 0) machineOpcode = OPCODE_ADDI;

    cmpResult = strcmp(mnemonic, ADD);
    if (cmpResult == 0) machineOpcode = OPCODE_ADD;

    cmpResult = strcmp(mnemonic, SUB);
    if (cmpResult == 0) machineOpcode = OPCODE_SUB;

    cmpResult = strcmp(mnemonic, MUL);
    if (cmpResult == 0) machineOpcode = OPCODE_MUL;

    cmpResult = strcmp(mnemonic, DIV);
    if (cmpResult == 0) machineOpcode = OPCODE_DIV;

    cmpResult = strcmp(mnemonic, LDW);
    if (cmpResult == 0) machineOpcode = OPCODE_LDW;

    cmpResult = strcmp(mnemonic, STW);
    if (cmpResult == 0) machineOpcode = OPCODE_STW;

    cmpResult = strcmp(mnemonic, BE);
    if (cmpResult == 0) machineOpcode = OPCODE_BE;

    cmpResult = strcmp(mnemonic, BNE);
    if (cmpResult == 0) machineOpcode = OPCODE_BNE;

    cmpResult = strcmp(mnemonic, BL);
    if (cmpResult == 0) machineOpcode = OPCODE_BL;

    cmpResult = strcmp(mnemonic, BG);
    if (cmpResult == 0) machineOpcode = OPCODE_BG;

    cmpResult = strcmp(mnemonic, BLE);
    if (cmpResult == 0) machineOpcode = OPCODE_BLE;

    cmpResult = strcmp(mnemonic, BGE);
    if (cmpResult == 0) machineOpcode = OPCODE_BGE;

    cmpResult = strcmp(mnemonic, B);
    if (cmpResult == 0) machineOpcode = OPCODE_B;

    cmpResult = strcmp(mnemonic, JAL);
    if (cmpResult == 0) machineOpcode = OPCODE_JAL;

    cmpResult = strcmp(mnemonic, JLR);
    if (cmpResult == 0) machineOpcode = OPCODE_JLR;

    cmpResult = strcmp(mnemonic, HALT);
    if (cmpResult == 0) machineOpcode = OPCODE_HALT;
       
    machineOpcode = machineOpcode << leftShift;
    *machineInstruction = *machineInstruction | machineOpcode;
}

void appendRegisterNumber(char* registerNumber, uint32_t* machineInstruction, uint32_t leftShift)
{
    uint32_t machineRegister = 0;
    int cmpResult = 0;

    cmpResult = strcmp(registerNumber, ACRO_R0);
    if (cmpResult == 0) machineRegister = MCRO_R0;
  
    cmpResult = strcmp(registerNumber, ACRO_R1);
    if (cmpResult == 0) machineRegister = MCRO_R1;

    cmpResult = strcmp(registerNumber, ACRO_R2);
    if (cmpResult == 0) machineRegister = MCRO_R2;

    cmpResult = strcmp(registerNumber, ACRO_R3);
    if (cmpResult == 0) machineRegister = MCRO_R3;

    cmpResult = strcmp(registerNumber, ACRO_R4);
    if (cmpResult == 0) machineRegister = MCRO_R4;

    cmpResult = strcmp(registerNumber, ACRO_R5);
    if (cmpResult == 0) machineRegister = MCRO_R5;

    cmpResult = strcmp(registerNumber, ACRO_R6);
    if (cmpResult == 0) machineRegister = MCRO_R6;

    cmpResult = strcmp(registerNumber, ACRO_R7);
    if (cmpResult == 0) machineRegister = MCRO_R7;

    cmpResult = strcmp(registerNumber, ACRO_R8);
    if (cmpResult == 0) machineRegister = MCRO_R8;

    cmpResult = strcmp(registerNumber, ACRO_R9);
    if (cmpResult == 0) machineRegister = MCRO_R9;

    cmpResult = strcmp(registerNumber, ACRO_R10);
    if (cmpResult == 0) machineRegister = MCRO_R10;

    cmpResult = strcmp(registerNumber, ACRO_R11);
    if (cmpResult == 0) machineRegister = MCRO_R11;

    cmpResult = strcmp(registerNumber, ACRO_R12);
    if (cmpResult == 0) machineRegister = MCRO_R12;

    cmpResult = strcmp(registerNumber, ACRO_R13);
    if (cmpResult == 0) machineRegister = MCRO_R13;

    cmpResult = strcmp(registerNumber, ACRO_R14);
    if (cmpResult == 0) machineRegister = MCRO_R14;

    cmpResult = strcmp(registerNumber, ACRO_R15);
    if (cmpResult == 0) machineRegister = MCRO_R15;

    cmpResult = strcmp(registerNumber, ACRO_SP);
    if (cmpResult == 0) machineRegister = MCRO_R14;

    machineRegister = machineRegister << leftShift;
    *machineInstruction = *machineInstruction | machineRegister;
}

void writeDssSize(FILE* fptr, char* dssSize)
{
    uint32_t machineCode = 0;
    char* value = dssSize + 1;

    if (dssSize[0] == 'b')
    {
        machineCode = (uint32_t)strtol(value, NULL, 2);
    }

    if (dssSize[0] == 'h')
    {
        machineCode = (uint32_t)strtol(value, NULL, 16);
    }

    size_t result = fwrite(&machineCode, sizeof(uint32_t), 1, fptr);
}

void processDataWordDeclaration(FILE* fptrWrite, char* token, uint32_t* dataWordCount)
{
    token = strtok(NULL, " ");

    if (token[0] == '[')
    {
        char* arrayValues = strtok(NULL, "\0");
        arrayValues++; //move past the opening brace {

        token++; //move past the opening bracket [

        writeDataWordArray(fptrWrite, token, arrayValues, dataWordCount);
    }
    else
    {
        writeDataWord(fptrWrite, token, dataWordCount);
    }
}

void writeDataWordArray(FILE* fptrWrite, char* token, char* arrayValues, uint32_t* dataWordCount)
{
    char* arraySizeString = strtok(token, "]");

    uint32_t arraySize = 0;

    if (arraySizeString[0] == 'b')
    {
        arraySize = (uint32_t)strtol(arraySizeString, NULL, 2);
    }
    else if (arraySizeString[0] == 'h')
    {
        arraySize = (uint32_t)strtol(arraySizeString, NULL, 16);
    }
    else
    {
        arraySize = (uint32_t)strtol(arraySizeString, NULL, 10);
    }

    char* arrayValue = strtok(arrayValues, ", }");

    for (uint32_t arrayElement = 0; arrayElement < arraySize; arrayElement++)
    {
        writeDataWord(fptrWrite, arrayValue, dataWordCount);

        arrayValue = strtok(NULL, ", }");
    }
}

void writeDataWord(FILE* fptrDataSegmentFile, char* dataWord, uint32_t* dataWordCount)
{
    uint32_t machineDataWord = 0;

    if (dataWord[0] == 'b')
    {
        char* value = dataWord + 1;

        machineDataWord = (uint32_t)strtol(value, NULL, 2);
    }
    else if (dataWord[0] == 'h')
    {
        char* value = dataWord + 1;

        machineDataWord = (uint32_t)strtol(value, NULL, 16);
    }
    else
    {
        char* value = dataWord;

        machineDataWord = (uint32_t)strtol(value, NULL, 10);
    }

    size_t result = fwrite(&machineDataWord, sizeof(uint32_t), 1, fptrDataSegmentFile);

    //result = fwrite(&machineCode, sizeof(uint32_t), 1, fptr);

    (*dataWordCount)++;
}

void append16BitImmediateValue(char* immediateValue, uint32_t* machineInstruction)
{
   uint16_t machineImmediate = 0;

   if (immediateValue[0] == 'b')
   {
       char* value = immediateValue + 1; //move 1 byte past base designator b

       machineImmediate = (uint16_t)strtol(value, NULL, 2);
   }
   else if (immediateValue[0] == 'h')
   {
       char* value = immediateValue + 1; //move 1 byte past base designator h

       machineImmediate = (uint16_t)strtol(value, NULL, 16);
   }
   else
   {
       char* value = immediateValue;

       machineImmediate = (uint16_t)strtol(value, NULL, 10);
   }
   
   *machineInstruction = *machineInstruction | machineImmediate;
}

void createMachineCodeFile(const char* const codeSegmentFilename, const char* const dataSegmentFilename, char* machineCodeFilename, uint32_t dataWordCount, uint32_t instructionCount)
{
    FILE* fptrDataSegmentFile = fopen(dataSegmentFilename, "rb");
    FILE* fptrCodeSegmentFile = fopen(codeSegmentFilename, "rb");
    FILE* fptrMachineCodeFile = fopen(machineCodeFilename, "wb");
    
    size_t result = fwrite(&dataWordCount, sizeof(uint32_t), 1, fptrMachineCodeFile);
    result = fwrite(&instructionCount, sizeof(uint32_t), 1, fptrMachineCodeFile);

    //Data segment file to machine code file

    for (uint32_t index = 0; index < dataWordCount; index++)
    {
        uint32_t dataSegmentWord = 0;
        result = fread(&dataSegmentWord, sizeof(uint32_t), 1, fptrDataSegmentFile);
        result = fwrite(&dataSegmentWord, sizeof(uint32_t), 1, fptrMachineCodeFile);
    }

    //Code segment file to machine code file

    for (uint32_t index = 0; index < instructionCount; index++)
    {
        uint32_t instruction = 0;
        result = fread(&instruction, sizeof(uint32_t), 1, fptrCodeSegmentFile);
        result = fwrite(&instruction, sizeof(uint32_t), 1, fptrMachineCodeFile);
    }

    fclose(fptrDataSegmentFile);
    fclose(fptrCodeSegmentFile);
    fclose(fptrMachineCodeFile);
}

