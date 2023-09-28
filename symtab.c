/* $Id: symtab.c,v 1.3 2023/09/14 22:29:44 leavens Exp $ */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "symtab.h"
#include "utilities.h"

// For a data structure, we use an array (for now),
// although a hash table would be better...

// size is also the index of the next element to allocate
static int size;
// The data structure is such that the first size entries contain actual data
static id_attrs entries[MAX_SYMTAB_SIZE];

// The symbol table's invariant
void symtab_okay()
{
    assert(0 <= size);
    assert(size < MAX_SYMTAB_SIZE);
    for (int i = size; i < MAX_SYMTAB_SIZE; i++) {
	assert(entries[i].name == NULL);
    }
}

// initialize the symbol table
void symtab_initialize()
{
    size = 0; // no data yet
    id_attrs none;
    none.name = NULL;
    none.file_loc = NULL;
    none.addr = 0;
    for (int i = 0; i < MAX_SYMTAB_SIZE; i++) {
	entries[i] = none;
    }
    symtab_okay();
}

// Return the number of mappings in this symbol table
unsigned int symtab_size() { return size; }

// Is this symbol table empty? (I.e., does it have not mappings?)
bool symtab_empty() { return size == 0; }

// Is this symbol table full? (I.e., can it not hold more mappings?)
// (i.e., is symtab_size() equal to MAX_SYMTAB_SIZE-1)?
bool symtab_full() { return size >= MAX_SYMTAB_SIZE; }

// Is the given name associated with some attributes?
bool symtab_defined(const char *name)
{
    id_attrs *v = symtab_lookup(name);
    return v != NULL;
}    

// Requires: !symtab_full
// Requires: !symtab_defined(attrs.name)
// Remember the given attributes (i.e., an association from attrs.name
// to the other parts of attrs)
void symtab_insert(id_attrs attrs)
{
    if (symtab_full()) {
	bail_with_error("The symtab is full!");
    }
    entries[size++] = attrs;
}

// if name == NULL or if name is not defined, return -1
// if name is defined in the table, return its index
static int find_index(const char *name)
{
    if (name == NULL) {
	return -1;
    }
    for (int i = 0; i < size; i++) {
	if (strcmp(entries[i].name, name) == 0) {
	    return i;
	}
    }
    return -1;
}


// Return (a pointer to) the attributes of the given name
// or NULL if there is no association for that name.
id_attrs *symtab_lookup(const char *name)
{
    int i = find_index(name);
    if (0 <= i) {
	return &entries[i];
    } else {
	return NULL;
    }
}

// iteration helpers
// iterations use an external key which is a name

// Start an iteration by returning the first name in the symbol table,
// return NULL if symtab_empty()
const char *symtab_first_name()
{
    if (symtab_empty()) {
	return NULL;
    }
    assert(0 < size);
    return entries[0].name;
}

// Are there more names defined in the symbol table after the given one?
// This returns false if name is NULL, if name is not defined,
// or if there are no more names following name in the symbol table
bool symtab_more_after(const char *name)
{
    int i = find_index(name);
    return (0 <= i && i < size);
}

// Requires: symtab_more_after(name);
// Return the next name defined in the symbol table after the given one,
// but return NULL if there are no more names
const char *symtab_next_name(const char *name)
{
    int i = find_index(name);
    if (i < 0 || i >= size) {
	return NULL;
    } else {
	return entries[i+1].name;
    }
}
