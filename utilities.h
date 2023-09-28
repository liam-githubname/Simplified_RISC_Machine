/* $Id: utilities.h,v 1.3 2023/09/16 16:23:10 leavens Exp $ */
#ifndef _UTILITIES_H
#define _UTILITIES_H
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// If NDEBUG is defined, do nothing, otherwise (when debugging)
// flush stderr and stdout, then print the message given on stderr,
// using printf formatting from the format string fmt.
// This function returns normally.
void debug_print(const char *fmt, ...);

// Format a string error message and print it using perror (for an OS error)
// then exit with a failure code, so a call to this does not return.
extern void bail_with_error(const char *fmt, ...);

// print a newline on out and flush out
extern void newline(FILE *out);

#endif
