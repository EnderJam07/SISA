#include <stdio.h>
#include <string.h>
#include "CPU.h"
#include "MachineOpcodes.h"
#include "InstructionFields.h"
#include "PrintingFormats.h"

#define PAUSE_AFTER_INSTRUCTION_PROCESSED 1

//PRIVATE FUNCTION PROTOTYPES
void CPUFetchInstruction(CPU* cpuPtr);
void CPUDecodeInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUExecuteInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUParseInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUSetMultiplexers(CPU* cpuPtr, Controller* controllerPtr);
void CPUReadALUSourceRegisters(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUExecuteALU(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUProcessUnconditionalBranchInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUProcessConditionalBranchInstruction(InstructionFields* instructionFieldsPtr, CPU* cpuPtr);
void CPUProcessBranchAndLinkInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUProcessBranchLinkRegisterInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUStoreDataInRegister(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUWriteDataToMemory(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUWriteDataToStackSegmentMemory(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUUpdatePC(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUPrintDecodedInformation(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr);
void CPUPrintExecutionStateInformation(CPU* cpuPtr, InstructionFields* instructionFieldsPtr, uint16_t startingMainMemoryAddress, uint32_t programSize, uint32_t mainMemorySize);
void CPUPrintInstruction(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr);
void CPUPrintBaseRegisters(CPU* cpuPtr);

//MODULE LEVEL VARIABLES
int pauseAfterInstructionProcessed = 0;

void CPUInitialize(CPU* cpuPtr, MainMemory* mainMemoryPtr)
{
    MMUInitialize(&cpuPtr->mmu, mainMemoryPtr);
}

int32_t CPUExecuteProgram(CPU* cpuPtr, uint32_t codeSegmentMemoryAddress, uint32_t dataSegmentMemoryAddress, uint32_t programSize, uint32_t mainMemorySize)
{
    uint32_t instructionCounter = 0;

    InstructionFields instructionFields;
    InstructionFields* instructionFieldsPtr;
    instructionFieldsPtr = &instructionFields;

    cpuPtr->codeSegmentRegister = codeSegmentMemoryAddress;
    cpuPtr->dataSegmentRegister = dataSegmentMemoryAddress;

    cpuPtr->PC_Register = cpuPtr->codeSegmentRegister;
    cpuPtr->registers.values[R14] = mainMemorySize;

    CPUPrintBaseRegisters(cpuPtr);
    RegistersPrintRegisterValues(&cpuPtr->registers);
    MainMemoryPrintValues(cpuPtr->mmu.mainMemoryPtr, 0, mainMemorySize);
    MainMemoryPrintCodeSegmentValues(cpuPtr->mmu.mainMemoryPtr, 0, cpuPtr->dataSegmentRegister);
    MainMemoryPrintDataSegmentValues(cpuPtr->mmu.mainMemoryPtr, cpuPtr->dataSegmentRegister, programSize);
    
    do
    {
        instructionCounter++;

        //Fetch Stage
        CPUFetchInstruction(cpuPtr);
        CPUPrintInstruction(cpuPtr, instructionCounter, instructionFieldsPtr);

        //Decode Stage
        CPUDecodeInstruction(cpuPtr, instructionFieldsPtr);
        CPUPrintDecodedInformation(cpuPtr, instructionCounter, instructionFieldsPtr);
        
        //Execute Stage
        CPUExecuteInstruction(cpuPtr, instructionFieldsPtr);
        CPUPrintExecutionStateInformation(cpuPtr, instructionFieldsPtr, codeSegmentMemoryAddress, programSize, mainMemorySize);
        
        //PC Update Stage
        CPUUpdatePC(cpuPtr, instructionFieldsPtr);

        // printf("************************** END PROCESSING INSTRUCTION **************************\n\n");

        if (pauseAfterInstructionProcessed == PAUSE_AFTER_INSTRUCTION_PROCESSED)
        {
            printf("Press enter key to execute next instruction...\n");

            int c = getchar();
        }

    } while (instructionFields.opcode != OPCODE_HALT_HEX);

    return cpuPtr->registers.values[R1];
}

void CPUFetchInstruction(CPU* cpuPtr)
{
    uint32_t instruction = cpuPtr->mmu.mainMemoryPtr->values[cpuPtr->PC_Register];

    cpuPtr->IR_Register = instruction;
}

//Determine the opcode, register addresses, and immediate value.
void CPUDecodeInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    CPUParseInstruction(cpuPtr, instructionFieldsPtr);

    ControllerSetControlRegister(&cpuPtr->controller, instructionFieldsPtr->opcode);

    ALUControllerSetControlRegisters(&cpuPtr->aluController, &cpuPtr->controller);

    CPUSetMultiplexers(cpuPtr, &cpuPtr->controller);
}

void CPUExecuteInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->controller.branchAndLink == BRANCH_AND_LINK_FLAG_ON)
    {
        CPUProcessBranchAndLinkInstruction(cpuPtr, instructionFieldsPtr);

        return;
    }

    if (cpuPtr->controller.branchLinkRegister == BRANCH_LINK_REGISTER_FLAG_ON)
    {
        CPUProcessBranchLinkRegisterInstruction(cpuPtr, instructionFieldsPtr);

        return;
    }
    
    if (cpuPtr->controller.uncondBranch == UNCOND_BRANCH_FLAG_ON)
    {
        CPUProcessUnconditionalBranchInstruction(cpuPtr, instructionFieldsPtr);
    }
    
    if (cpuPtr->controller.uncondBranch == UNCOND_BRANCH_FLAG_OFF) 
    {
        //All other instructions require the ALU

        CPUReadALUSourceRegisters(cpuPtr, instructionFieldsPtr);

        CPUExecuteALU(cpuPtr, instructionFieldsPtr);

        if (cpuPtr->controller.condBranch == COND_BRANCH_FLAG_ON)
        {
            CPUProcessConditionalBranchInstruction(instructionFieldsPtr, cpuPtr);
        }
        
        if (cpuPtr->controller.condBranch == COND_BRANCH_FLAG_OFF)
        {
            if (cpuPtr->controller.regWrite == REG_WRITE_FLAG_ON)
                CPUStoreDataInRegister(cpuPtr, instructionFieldsPtr);

            if (cpuPtr->controller.memWrite == MEM_WRITE_FLAG_ON)
                CPUWriteDataToMemory(cpuPtr, instructionFieldsPtr);
        }
    }
}

void CPUUpdatePC(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    //TODO
    if (cpuPtr->pcSourceSelector == PC_SOURCE_SELECT_BRANCH)
    {
        //TODO: Move to PC Branch-Offset Adder

        //Assembly statement branch offset immediate values are in bytes.
        //Memory is word-addressable (4 bytes per word).
        //Instructions are 4-byte words. 
        //Convert to word offsets by dividing by four.
        int32_t byteToWordAdjustment = instructionFieldsPtr->immediate >> 2;

        cpuPtr->PC_Register = cpuPtr->PC_Register + byteToWordAdjustment;
    }
    else if (cpuPtr->controller.branchLinkRegister == BRANCH_LINK_REGISTER_FLAG_ON)
    {
        cpuPtr->PC_Register = cpuPtr->registers.values[R15];
    }
    else
    {
        //TODO
        cpuPtr->PC_Register++; //Move to PC Adder
    }
}

void CPUProcessBranchAndLinkInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    cpuPtr->registers.values[R15] = cpuPtr->PC_Register + 1;

    cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
}

void CPUProcessBranchLinkRegisterInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    //Nothing to do here.
}

void CPUProcessUnconditionalBranchInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
}

void CPUProcessConditionalBranchInstruction(InstructionFields* instructionFieldsPtr, CPU* cpuPtr)
{
    if (instructionFieldsPtr->opcode == OPCODE_BE_HEX)
    {
        if (cpuPtr->alu.zeroFlag == ALU_FLAG_ON)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BNE_HEX)
    {
        if (cpuPtr->alu.zeroFlag == ALU_FLAG_OFF)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BL_HEX)
    {
        if (cpuPtr->alu.lessZeroFlag == ALU_FLAG_ON)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BG_HEX)
    {
        if (cpuPtr->alu.greaterZeroFlag == ALU_FLAG_ON)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BLE_HEX)
    {
        if ((cpuPtr->alu.zeroFlag == ALU_FLAG_ON) || (cpuPtr->alu.lessZeroFlag == ALU_FLAG_ON))
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BGE_HEX)
    {
        if ((cpuPtr->alu.zeroFlag == ALU_FLAG_ON) || (cpuPtr->alu.greaterZeroFlag == ALU_FLAG_ON))
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }
}

void CPUParseInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    uint32_t result = cpuPtr->IR_Register & OPCODE_BITMASK;
    instructionFieldsPtr->opcode = (uint8_t)(result >> OPCODE_BIT_SHIFT);

    switch (instructionFieldsPtr->opcode)
    {
        case OPCODE_LDWI_HEX:
        {
            result = cpuPtr->IR_Register & RD_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RD_FIELD_BIT_SHIFT);

            instructionFieldsPtr->RM = 0;

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & IMM_FIELD_BITMASK;
            instructionFieldsPtr->immediate = (int16_t)result;

            break;
        }
        case OPCODE_ADDI_HEX:
        {
            result = cpuPtr->IR_Register & RD_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RD_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RM_Field_BITMASK;
            instructionFieldsPtr->RM = (uint8_t)(result >> RM_FIELD_BIT_SHIFT);

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & IMM_FIELD_BITMASK;
            instructionFieldsPtr->immediate = (int16_t)result;

            break;
        }
        case OPCODE_ADD_HEX:
        case OPCODE_SUB_HEX:
        case OPCODE_MUL_HEX:
        case OPCODE_DIV_HEX:
        {
            result = cpuPtr->IR_Register & RD_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RD_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RM_Field_BITMASK;
            instructionFieldsPtr->RM = (uint8_t)(result >> RM_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RN_Field_BITMASK;
            instructionFieldsPtr->RN = (uint8_t)(result >> RN_FIELD_BIT_SHIFT);

            instructionFieldsPtr->immediate = 0;

            break;
        }
        case OPCODE_LDW_HEX:
        case OPCODE_STW_HEX:
        {
            result = cpuPtr->IR_Register & RT_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RT_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RM_Field_BITMASK;
            instructionFieldsPtr->RM = (uint8_t)(result >> RM_FIELD_BIT_SHIFT);

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & BASE_OFFSET_FIELD_BITMASK;
            instructionFieldsPtr->immediate = (int16_t)result;

            break;
        }
        case OPCODE_BE_HEX:
        case OPCODE_BNE_HEX:
        case OPCODE_BL_HEX:
        case OPCODE_BG_HEX:
        case OPCODE_BLE_HEX:
        case OPCODE_BGE_HEX:
        {
            result = cpuPtr->IR_Register & RT_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RT_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RM_Field_BITMASK;
            instructionFieldsPtr->RM = (uint8_t)(result >> RM_FIELD_BIT_SHIFT);

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & CBRANCH_PC_OFFSET_FIELD_BITMASK;
            instructionFieldsPtr->immediate = (int16_t)result;

            break;
        }
        case OPCODE_B_HEX:
        {
            instructionFieldsPtr->RD_RT = 0;

            instructionFieldsPtr->RM = 0;

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & UBRANCH_PC_OFFSET_FIELD_BITMASK;

            instructionFieldsPtr->immediate = (int16_t)result;

            break;
        }
        case OPCODE_JAL_HEX:
        {
            instructionFieldsPtr->RD_RT = 0;

            instructionFieldsPtr->RM = 0;

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & UBRANCH_PC_OFFSET_FIELD_BITMASK;

            instructionFieldsPtr->immediate = (int16_t)result;

            break;
        }
        case OPCODE_JLR_HEX:
        {
            instructionFieldsPtr->RD_RT = 0;

            instructionFieldsPtr->RM = 0;

            instructionFieldsPtr->RN = 0;

            instructionFieldsPtr->immediate = (int16_t)0;

            break;
        }
        case OPCODE_HALT_HEX:
        {
            instructionFieldsPtr->RD_RT = 0;
            instructionFieldsPtr->RM = 0;
            instructionFieldsPtr->RN = 0;
            instructionFieldsPtr->immediate = 0;

            break;
        }
        default:
        {
            instructionFieldsPtr->RD_RT = 0;
            instructionFieldsPtr->RM = 0;
            instructionFieldsPtr->RN = 0;
            instructionFieldsPtr->immediate = 0;

            break;
        }
    }
}

void CPUSetMultiplexers(CPU* cpuPtr, Controller* controllerPtr)
{
    cpuPtr->aluDataInputBSourceMultiplexer.dataSelector = controllerPtr->aluInputBSourceSelector;
    cpuPtr->registerWriteDataSourceMultiplexer.dataSelector = controllerPtr->regWriteSourceSelector;
    cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_NORMAL;
}

void CPUReadALUSourceRegisters(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->aluDataInputBSourceMultiplexer.dataSelector == ALU_INPUT_B_SOURCE_SELECT_REG)
    {
        if (cpuPtr->controller.condBranch == COND_BRANCH_FLAG_OFF)
        {
            cpuPtr->alu.dataInputA = cpuPtr->registers.values[instructionFieldsPtr->RM];
            cpuPtr->alu.dataInputB = cpuPtr->registers.values[instructionFieldsPtr->RN];
        }
        else
        {
            cpuPtr->alu.dataInputA = cpuPtr->registers.values[instructionFieldsPtr->RD_RT];
            cpuPtr->alu.dataInputB = cpuPtr->registers.values[instructionFieldsPtr->RM];
        }
    }
    else if (cpuPtr->aluDataInputBSourceMultiplexer.dataSelector == ALU_INPUT_B_SOURCE_SELECT_IMM)
    {   
        cpuPtr->alu.dataInputA = cpuPtr->registers.values[instructionFieldsPtr->RM];
        cpuPtr->alu.dataInputB = instructionFieldsPtr->immediate;
    }
}

void CPUExecuteALU(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{   
    cpuPtr->alu.zeroFlag = ALU_FLAG_OFF;
    cpuPtr->alu.lessZeroFlag = ALU_FLAG_OFF;
    cpuPtr->alu.greaterZeroFlag = ALU_FLAG_OFF;

    if (cpuPtr->aluController.aluOpAddi == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA + cpuPtr->alu.dataInputB;

    if (cpuPtr->aluController.aluOpAdd == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA + cpuPtr->alu.dataInputB;

    if (cpuPtr->aluController.aluOpSub == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA - cpuPtr->alu.dataInputB;

    if (cpuPtr->aluController.aluOpMul == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA * cpuPtr->alu.dataInputB;

    if (cpuPtr->aluController.aluOpDiv == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA / cpuPtr->alu.dataInputB;

    if (cpuPtr->alu.dataOutput == 0) cpuPtr->alu.zeroFlag = ALU_FLAG_ON;
    if (cpuPtr->alu.dataOutput < 0) cpuPtr->alu.lessZeroFlag = ALU_FLAG_ON;
    if (cpuPtr->alu.dataOutput > 0) cpuPtr->alu.greaterZeroFlag = ALU_FLAG_ON;
}

void CPUStoreDataInRegister(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->controller.regWrite == REG_WRITE_FLAG_ON)
    {
        if (cpuPtr->registerWriteDataSourceMultiplexer.dataSelector == REG_WRITE_SOURCE_SELECT_ALU)
        {
            cpuPtr->registers.values[instructionFieldsPtr->RD_RT] = cpuPtr->alu.dataOutput;
        }

        if (cpuPtr->registerWriteDataSourceMultiplexer.dataSelector == REG_WRITE_SOURCE_SELECT_MEM)
        {   
            //Word-addressable memory (4-bytes per word):
            //With word-addressable memory, for the LDW and STW instructions, the base-offset 
            //immediate value represents a word offset. Since memory is word addressable, 
            //this base-offset immediate value can be added to the RM base-register to generate 
            //the effective-address.
            
            //TODO -- Byte-addressable memory:
            //With byte-addressable memory, for the LDW and STW instructions, the base-offset 
            //immediate value would represents byte offset. Currently, since main memory is 
            //word-addressable and not byte addressable, this base-offset immediate value 
            //CAN NOT be directly added to the RM base-register to generate an effective-address,
            //since dividing the ALU output value by 4 would produce the incorrect effective-address
            //because the RM base-register would be divided by 4 also!
            //Therefore, this won't work with byte-addressable memory:
            //cpuPtr->alu.dataOutput >>= BYTE_TO_WORD_BASE_OFFSET_BIT_SHIFT;
            //To use byte-addressable memory, main memory would need to be implmented as an array of bytes.

            uint32_t memoryAddress = 0;

            if (instructionFieldsPtr->RD_RT == R14 || instructionFieldsPtr->RM == R14)
            {
                memoryAddress = cpuPtr->alu.dataOutput;
            }
            else
            {
                memoryAddress = cpuPtr->dataSegmentRegister + cpuPtr->alu.dataOutput;
            }

            uint32_t mainMemoryValue = cpuPtr->mmu.mainMemoryPtr->values[memoryAddress];

            cpuPtr->registers.values[instructionFieldsPtr->RD_RT] = mainMemoryValue;
        }
    }
}

void CPUWriteDataToMemory(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    //TODO -- See comment above.
   
    uint32_t memoryAddress = 0;

    if (instructionFieldsPtr->RD_RT == R14 || instructionFieldsPtr->RM == R14)
    {
        memoryAddress = cpuPtr->alu.dataOutput;
    }
    else
    {
        memoryAddress = cpuPtr->dataSegmentRegister + cpuPtr->alu.dataOutput;
    }

    cpuPtr->mmu.mainMemoryPtr->values[memoryAddress] = cpuPtr->registers.values[instructionFieldsPtr->RD_RT];
}

void CPUWriteDataToStackSegmentMemory(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    //TODO -- See comment above.

    uint32_t memoryAddress = cpuPtr->dataSegmentRegister + cpuPtr->alu.dataOutput;

    cpuPtr->mmu.mainMemoryPtr->values[memoryAddress] = cpuPtr->registers.values[instructionFieldsPtr->RD_RT];
}

void CPUPrintInstruction(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr)
{
    uint32_t byteAdjustedPc = cpuPtr->PC_Register << 2;

    printf("%2d: ", instructionCounter);

    // printf("Address (PC):      Hex: 0x%08X\n", byteAdjustedPc);
    // printf("Instruction (IR):  Hex: 0x%08X ", cpuPtr->IR_Register);
    // printf("Binary: ");
    // printf(THIRTY_TWO_BIT_BYTE_TO_BINARY_PATTERN, THIRTY_TWO_BIT_BYTE_TO_BINARY(cpuPtr->IR_Register));
    // printf("\n\n");
}

void CPUPrintBaseRegisters(CPU* cpuPtr)
{
    uint32_t byteAdjusted = cpuPtr->codeSegmentRegister;

    printf("Code Segment Base Register (CSBR): 0x%08X\n", byteAdjusted);

    byteAdjusted = cpuPtr->dataSegmentRegister << 2;

    printf("Data Segment Base Register (DSBR): 0x%08X\n\n", byteAdjusted);
}

void CPUPrintDecodedInformation(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr)
{
    InstructionFieldsPrintState(instructionFieldsPtr);
}

void CPUPrintExecutionStateInformation(CPU* cpuPtr, InstructionFields* instructionFieldsPtr, 
    uint16_t startingMainMemoryAddress, uint32_t programSize, uint32_t mainMemorySize)
{
    ControllerPrintControlRegister(&cpuPtr->controller);
    ALUControllerPrintControlRegister(&cpuPtr->aluController);
    RegistersPrintRegisterValues(&cpuPtr->registers);
    MainMemoryPrintValues(cpuPtr->mmu.mainMemoryPtr, 0, mainMemorySize);
    MainMemoryPrintCodeSegmentValues(cpuPtr->mmu.mainMemoryPtr, 0, cpuPtr->dataSegmentRegister);
    MainMemoryPrintDataSegmentValues(cpuPtr->mmu.mainMemoryPtr, cpuPtr->dataSegmentRegister, programSize);
}

void CPUSetOptions(int argc, char* argv[])
{
    for (int commandLineParameter = 1; commandLineParameter < argc; commandLineParameter++)
    {
        int result = strcmp(argv[commandLineParameter], "-step");

        if (result == 0) pauseAfterInstructionProcessed = PAUSE_AFTER_INSTRUCTION_PROCESSED;
    }
}

