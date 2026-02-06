#pragma once

/*******************************************************************************
* Module:       ALU
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:
* Description:
*******************************************************************************/

#include <stdint.h>

#define ALU_FLAG_OFF	0
#define ALU_FLAG_ON		1

typedef struct
{
	int32_t dataInputA;
	int32_t dataInputB;
	int32_t dataOutput;

	bool zeroFlag;
	bool lessZeroFlag;
	bool greaterZeroFlag;
	
}ALU;