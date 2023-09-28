/* $Id: utilities.c,v 1.3 2023/09/16 16:23:10 leavens Exp $ */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include "utilities.h"

// to turn off debugging support (assertions and debug_print)
// define the symbol NDEBUG (by writing uncommenting the following)
// #define NDEBUG

#ifdef NDEBUG
#define debug_print() ((void)0)
#else
// otherwise debugging is on, and debug_print is defined as follows...
// (note that assert is a macro defined in <assert.h>
static void vdebug_print(const char *fmt, va_list args);

// If debugging is false, do nothing, otherwise (when debugging)
// flush stderr and stdout, then print the message given on stderr,
// using printf formatting from the format string fmt.
// This function returns normally.
void debug_print(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vdebug_print(fmt, args);
}

// The variadic version of debug_print
static void vdebug_print(const char *fmt, va_list args)
{
    // flush output streams to synchronize outputs
    fflush(stdout);
    fflush(stderr);
    vfprintf(stderr, fmt, args);
    fflush(stderr);
}
#endif

static void vbail_with_error(const char* fmt, va_list args);

// Format a string error message and print it followed by a newline on stderr
// using perror (for an OS error, if the errno is not 0)
// then exit with a failure code, so a call to this does not return.
void bail_with_error(const char *fmt, ...)
{
    fflush(stdout); // flush so output comes after what has happened already
    va_list(args);
    va_start(args, fmt);
    vbail_with_error(fmt, args);
}

// The variadic version of bail_with_error
static void vbail_with_error(const char* fmt, va_list args)
{
    extern int errno;
    char buff[2048];
    vsprintf(buff, fmt, args);
    if (errno != 0) {
	perror(buff);
    } else {
	fprintf(stderr, "%s\n", buff);
    }
    fflush(stderr);
    exit(EXIT_FAILURE);
}

// print a newline on out and flush out
void newline(FILE *out)
{
    fprintf(out, "\n");
    fflush(out);
}
