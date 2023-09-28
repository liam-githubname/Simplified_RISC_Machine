/* $Id: bof.h,v 1.11 2023/09/16 16:23:10 leavens Exp $ */
// Binary Object File Format (for the SRM)
#ifndef _BOF_H
#define _BOF_H
#include <stdio.h>
#include <stdint.h>
#include "machine_types.h"

#define MAGIC_BUFFER_SIZE 4

typedef struct { // Field magic should be "BOF" (with the null char)
    char     magic[MAGIC_BUFFER_SIZE];
    word_type text_start_address;  // byte address to start running (PC)
    word_type text_length;
    word_type data_start_address;  // byte address of static data (GP)
    word_type data_length;
    word_type stack_bottom_addr;   // byte address of stack "bottom" (FP)
} BOFHeader;

// a type for Binary Output Files
typedef struct {
    FILE *fileptr;
    const char *filename;
} BOFFILE;

// Open filename for reading as a binary file
// Exit the program with an error if this fails,
// otherwise return the FILE pointer to the open file.
extern BOFFILE bof_read_open(const char *filename);

// Requires: bf is open for reading in binary and has
//           at least BYTES_PER_WORD bytes in it
// Return the next word from bf
extern word_type bof_read_word(BOFFILE bf);

// Requires: bf is open for reading in binary and
// buf is of size at least bytes
// Read the given number of bytes into buf and return the number of bytes read
size_t bof_read_bytes(BOFFILE bf, size_t bytes, void *buf);

// Requires: bf is open for reading in binary
// Read the header of bf as a BOFHeader and return that header
// If any errors are encountered, exit with an error message.
extern BOFHeader bof_read_header(BOFFILE);

// Open filename for writing as a binary file
// Exit the program with an error if this fails,
// otherwise return the BOFFILE for it.
extern BOFFILE bof_write_open(const char *filename);

// Requres: bf is open
// Close the given binary file
// Exit the program with an error if this fails.
extern void bof_close(BOFFILE bf);

// Requires: f (which is accessed through filename)
//           is open for writing in binary
//           and the size of buf is at least BYTES_PER_WORD bytes
// Write the given word into f.
// Exit the program with an error if this fails.
extern void bof_write_word(BOFFILE bf, word_type w);

// Requires: bf is open for writing in binary
//           and the size of buf is at least bytes
// Write the given number of bytes from buf into f.
// Exit the program with an error if this fails.
extern void bof_write_bytes(BOFFILE bf, size_t bytes,
			    const void *buf);

// Requires: bf is open for writing in binary
// Write the given header to f
// Exit the program with an error if this fails.
void bof_write_header(BOFFILE bf, const BOFHeader hdr);
// The following line is for the SRM manual document
// ...
#endif
