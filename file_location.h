/* $Id: file_location.h,v 1.2 2023/09/09 20:45:49 leavens Exp $ */
#ifndef _FILE_LOCATION_H
#define _FILE_LOCATION_H

// location in a source file (useful for error messages)
typedef struct {
    const char *filename;
    unsigned int line; // of first token
} file_location;

// Requires: filename != NULL
// Return a (pointer to a) fresh file_location with the given
// information
extern file_location *file_location_make(const char *filename,
					 unsigned int line);

// Requires: fl != NULL
// Return a (pointer to a) fresh copy of fl
extern file_location *file_location_copy(file_location *fl);

#endif
