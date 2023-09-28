// $Id: regname.h,v 1.2 2023/09/18 02:24:31 leavens Exp $
#ifndef _REGNAME_H
#define _REGNAME_H
#define NUM_REGISTERS 32

// some important register numbers in the ISA
#define GP 28
#define SP 29
#define FP 30
#define RA 31

// requires 0 <= n && n < NUM_REGISTERS
// Return the standard symbolic name for n
extern const char *regname_get(int n);
#endif

