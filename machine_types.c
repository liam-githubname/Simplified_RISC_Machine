// $Id: machine_types.c,v 1.2 2023/09/21 18:48:31 leavens Exp $
#include "machine_types.h"

// Return the sign extended equivalent of i
int machine_types_sgnExt(immediate_type i) {
    short int s = i;  // preserves the value, now it is signed
    return (int) s;   // extends it to the width of an int
}

// Return the zero extended equivalent of i
unsigned int machine_types_zeroExt(immediate_type i) {
    return (int) (0x0000FFFF & i);
}

// Return the offset given by o, which is the sign extension of (o times 4)
int machine_types_formOffset(immediate_type o) {
    return machine_types_sgnExt(o << 2);
}

// Return an address formed by shifting a left by 2 bits
// and concatenating that with the high-order 4 bits of PC.
address_type machine_types_formAddress(address_type PC, address_type a) {
    return (PC & 0xF0000000) | (a << 2);
}
