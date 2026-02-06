#pragma once

/*******************************************************************************
* Module:       PrintingFormats
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:
* Description:
*******************************************************************************/

//PRINTING FORMATS
#define THIRTY_TWO_BIT_BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c"
#define SIXTEEN_BIT_BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define TWELVE_BIT_BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c%c%c%c%c"
#define EIGHT_BIT_BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define FOUR_BIT_NIBBLE_TO_BINARY_PATTERN "%c%c%c%c"
#define TW0_BITS_TO_BINARY_PATTERN "%c%c"
#define ONE_BIT_TO_BINARY_PATTERN "%c"

#define THIRTY_TWO_BIT_BYTE_TO_BINARY(byte)  \
    (byte & 0x80000000 ? '1' : '0'), \
    (byte & 0x40000000 ? '1' : '0'), \
    (byte & 0x20000000 ? '1' : '0'), \
    (byte & 0x10000000 ? '1' : '0'), \
    (byte & 0x08000000 ? '1' : '0'), \
    (byte & 0x04000000 ? '1' : '0'), \
    (byte & 0x02000000 ? '1' : '0'), \
    (byte & 0x01000000 ? '1' : '0'), \
    (byte & 0x00800000 ? '1' : '0'), \
    (byte & 0x00400000 ? '1' : '0'), \
    (byte & 0x00200000 ? '1' : '0'), \
    (byte & 0x00100000 ? '1' : '0'), \
    (byte & 0x00080000 ? '1' : '0'), \
    (byte & 0x00040000 ? '1' : '0'), \
    (byte & 0x00020000 ? '1' : '0'), \
    (byte & 0x00010000 ? '1' : '0'), \
    (byte & 0x00008000 ? '1' : '0'), \
    (byte & 0x00004000 ? '1' : '0'), \
    (byte & 0x00002000 ? '1' : '0'), \
    (byte & 0x00001000 ? '1' : '0'), \
    (byte & 0x00000800 ? '1' : '0'), \
    (byte & 0x00000400 ? '1' : '0'), \
    (byte & 0x00000200 ? '1' : '0'), \
    (byte & 0x00000100 ? '1' : '0'), \
    (byte & 0x00000080 ? '1' : '0'), \
    (byte & 0x00000040 ? '1' : '0'), \
    (byte & 0x00000020 ? '1' : '0'), \
    (byte & 0x00000010 ? '1' : '0'), \
    (byte & 0x00000008 ? '1' : '0'), \
    (byte & 0x00000004 ? '1' : '0'), \
    (byte & 0x00000002 ? '1' : '0'), \
    (byte & 0x00000001 ? '1' : '0') 

#define SIXTEEN_BIT_BYTE_TO_BINARY(byte)  \
    (byte & 0x8000 ? '1' : '0'), \
    (byte & 0x4000 ? '1' : '0'), \
    (byte & 0x2000 ? '1' : '0'), \
    (byte & 0x1000 ? '1' : '0'), \
    (byte & 0x0800 ? '1' : '0'), \
    (byte & 0x0400 ? '1' : '0'), \
    (byte & 0x0200 ? '1' : '0'), \
    (byte & 0x0100 ? '1' : '0'), \
    (byte & 0x0080 ? '1' : '0'), \
    (byte & 0x0040 ? '1' : '0'), \
    (byte & 0x0020 ? '1' : '0'), \
    (byte & 0x0010 ? '1' : '0'), \
    (byte & 0x0008 ? '1' : '0'), \
    (byte & 0x0004 ? '1' : '0'), \
    (byte & 0x0002 ? '1' : '0'), \
    (byte & 0x0001 ? '1' : '0') 

#define TWELVE_BIT_BYTE_TO_BINARY(byte)  \
    (byte & 0x0800 ? '1' : '0'), \
    (byte & 0x0400 ? '1' : '0'), \
    (byte & 0x0200 ? '1' : '0'), \
    (byte & 0x0100 ? '1' : '0'), \
    (byte & 0x0080 ? '1' : '0'), \
    (byte & 0x0040 ? '1' : '0'), \
    (byte & 0x0020 ? '1' : '0'), \
    (byte & 0x0010 ? '1' : '0'), \
    (byte & 0x0008 ? '1' : '0'), \
    (byte & 0x0004 ? '1' : '0'), \
    (byte & 0x0002 ? '1' : '0'), \
    (byte & 0x0001 ? '1' : '0') 

#define EIGHT_BIT_BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define FOUR_BIT_NIBBLE_TO_BINARY(byte)  \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define TWO_BITS_TO_BINARY(byte)  \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define ONE_BIT_TO_BINARY(byte)  \
  (byte & 0x01 ? '1' : '0') 
