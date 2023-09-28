/* $Id: symtab.h,v 1.2 2023/09/14 21:19:33 leavens Exp $ */
#ifndef _SYMTAB_H
#define _SYMTAB_H

#include <stdbool.h>
#include "id_attrs.h"

// Maximum number of names/attributes that can be stored in a symboltable
#define MAX_SYMTAB_SIZE 1024

// initialize the symbol table
extern void symtab_initialize();

// Return the number of mappings in this symbol table
extern unsigned int symtab_size();

// Is this symbol table empty? (I.e., does it have not mappings?)
extern bool symtab_empty();

// Is this symbol table full? (I.e., can it not hold more mappings?)
// (i.e., is symtab_size() equal to MAX_SYMTAB_SIZE-1)?
extern bool symtab_full();

// Is the given name associated with some attributes?
extern bool symtab_defined(const char *name);

// Requires: !symtab_defined(name) && attrs != NULL
// Remember the given attributes (i.e., an association from attrs->name
// to the other parts of *attrs)
extern void symtab_insert(id_attrs attrs);

// Return a pointer to the attributes of the given name
// or NULL if there is no association for that name.
extern id_attrs *symtab_lookup(const char *name);

// Start an iteration by returning the first name in the symbol table,
// return NULL if symtab_empty()
extern const char *symtab_first_name();

// Are there more names defined in the symbol table after the given one?
// This returns false if name is NULL.
extern bool symtab_more_after(const char *name);

// Requires: symtab_more_after(name);
// Return the next name defined in the symbol table after the given one,
// but return NULL if there are no more names
extern const char *symtab_next_name(const char *name);
#endif
