/* $Id: bof.c,v 1.9 2023/09/17 20:47:27 leavens Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bof.h"
#include "utilities.h"

// a type for treating bytes as a word
typedef union {
    unsigned char buf[BYTES_PER_WORD];
    word_type w;
} word_pun_t;

// Open filename for reading as a binary file
// Exit the program with an error if this fails,
// otherwise return the BOFFILE struct for the file
BOFFILE bof_read_open(const char *filename) {
    BOFFILE bf;
    bf.fileptr = fopen(filename, "rb");
    bf.filename = filename;

    if (bf.fileptr == NULL) {
	bail_with_error("Error opening file for reading: %s", filename);
    }
    
    return bf;
}

// Requires: bf is open for reading in binary and has
//           at least BYTES_PER_WORD bytes in it
// Return the next word from bf
word_type bof_read_word(BOFFILE bf)
{
    word_pun_t b;
    size_t bytes_read = bof_read_bytes(bf, BYTES_PER_WORD, b.buf);
    if (bytes_read == 0) {
	bail_with_error("Cannot read a word from %s (got %d bytes: 0x%x), the file is at EOF: %d",
			bf.filename, bytes_read, b.w, feof(bf.fileptr));
    }
    return b.w;
}

// Requires: bf.fileptr is open for reading in binary
// and buf is of size at least bytes
// Read the given number of bytes into buf and return the number of bytes read
size_t bof_read_bytes(BOFFILE bf, size_t bytes, void *buf) {
    int elems_read = fread(buf, bytes, 1, bf.fileptr);
    return elems_read;
}

// Requires: bf is open for reading in binary
// Read the header of bf as a BOFHeader and return that header
// If any errors are encountered, exit with an error message.
BOFHeader bof_read_header(BOFFILE bf) {
    BOFHeader ret;
    size_t rd = fread(&ret, sizeof(ret), 1, bf.fileptr);
    if (rd != 1) {
	bail_with_error("Cannot read header from %s", bf.filename);
    }
    return ret;
    /*
    bof_read_bytes(bf, MAGIC_BUFFER_SIZE, &ret.magic);
    if (strncmp(ret.magic, "BOF", MAGIC_BUFFER_SIZE) != 0) {
	bail_with_error("File %s is not a BOF format file, bad magic number!",
			bf.filename);
    }
    bof_read_bytes(bf, BYTES_PER_WORD, &ret.text_start_address);
    bof_read_bytes(bf, BYTES_PER_WORD, &ret.text_length);
    bof_read_bytes(bf, BYTES_PER_WORD, &ret.data_start_address);
    bof_read_bytes(bf, BYTES_PER_WORD, &ret.data_length);
    bof_read_bytes(bf, BYTES_PER_WORD, &ret.stack_bottom_addr);
    return ret;
    */
}

// Open filename for writing as a binary file
// Exit the program with an error if this fails,
// otherwise return the BOFFILE for it.
BOFFILE bof_write_open(const char *filename) {
    BOFFILE bf;
    bf.fileptr = fopen(filename, "wb");
    bf.filename = filename;

    if (bf.fileptr == NULL) {
	bail_with_error("Error opening file for writing: %s", filename);
    }
    
    return bf;
}

// Requres: bf is open
// Close the given binary file
// Exit the program with an error if this fails.
void bof_close(BOFFILE bf)
{
    if (fclose(bf.fileptr) != 0) {
	bail_with_error("Could not close %s", bf.filename);
    }
}


// Requires: bf is open for writing in binary
//           and the size of buf is at least BYTES_PER_WORD bytes
// Write the given word into bf
// Exit the program with an error if this fails.
extern void bof_write_word(BOFFILE bf, word_type w)
{
    word_pun_t b;
    b.w = w;
    bof_write_bytes(bf, BYTES_PER_WORD, b.buf);
}

// Requires: bf is open for writing in binary
//           and the size of buf is at least bytes
// Write the given number of bytes from buf into f.
// Exit the program with an error if this fails.
void bof_write_bytes(BOFFILE bf, size_t bytes,
		     const void *buf) {
    size_t wr = fwrite(buf, bytes, 1, bf.fileptr);
    if (wr != 1) {
	bail_with_error("Cannot write %u bytes to %s", bytes, bf.filename);
    }
}

// Requires: bf is open for writing in binary
// Write the given header to f
// Exit the program with an error if this fails.
void bof_write_header(BOFFILE bf, const BOFHeader hdr) {
    size_t wr = fwrite(&hdr, sizeof(BOFHeader), 1, bf.fileptr);
    if (wr != 1) {
	bail_with_error("Canot write header to %s", bf.filename);
    }
    /*
    bof_write_bytes(bf, MAGIC_BUFFER_SIZE, hdr.magic);
    bof_write_bytes(bf, BYTES_PER_WORD, &hdr.text_start_address);
    bof_write_bytes(bf, BYTES_PER_WORD, &hdr.text_length);
    bof_write_bytes(bf, BYTES_PER_WORD, &hdr.data_start_address);
    bof_write_bytes(bf, BYTES_PER_WORD, &hdr.data_length);
    bof_write_bytes(bf, BYTES_PER_WORD, &hdr.stack_bottom_addr);
    */
}
