// $Id: machine_types.h,v 1.8 2023/09/18 02:24:31 leavens Exp $
// Machine Types for the Simplified Risc Machine (SRM)
#ifndef _MACHINE_TYPES_H
#define _MACHINE_TYPES_H

// registers encoded in instructions
typedef unsigned short reg_num_type;

// type of shift values encoded in immediate instructions
typedef unsigned short shift_type;

// type of functions encoded in instructions
typedef unsigned short func_type;

// type of immediate operands encoded in instructions
typedef unsigned short immediate_type;

// type of addresses
typedef unsigned int address_type;

// type of bytes
typedef unsigned char byte_type;

// type of machine words
typedef int word_type;

#define BYTES_PER_WORD 4

// Return the sign extended equivalent of i
extern int machine_types_sgnExt(immediate_type i);

// Return the zero extended equivalent of i
extern unsigned int machine_types_zeroExt(immediate_type i);

// Return the offset given by o, which is the sign extension of (o times 4)
extern int machine_types_formOffset(immediate_type o);

// Return an address formed by shifting a left by 2 bits
// and concatenating that with the high-order 4 bits of PC.
extern address_type machine_types_formAddress(address_type PC, address_type a);

#endif
