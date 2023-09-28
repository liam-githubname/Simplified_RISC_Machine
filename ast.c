/* $Id: ast.c,v 1.21 2023/09/17 20:47:27 leavens Exp $ */
#include <string.h>
#include <stdlib.h>
#include "utilities.h"
#include "ast.h"
#include "lexer.h"

// Return the file location from an AST
file_location *ast_file_loc(AST t) {
    return t.generic.file_loc;
}

// Return the filename from the AST t
const char *ast_filename(AST t) {
    return ast_file_loc(t)->filename;
}

// Return the line number from the AST t
unsigned int ast_line(AST t) {
    return ast_file_loc(t)->line;
}

// Return the type tag of the AST t
AST_type ast_type_tag(AST t) {
    return t.generic.type_tag;
}

// Return a pointer to a fresh copy of t
// that has been allocated on the heap
AST *ast_heap_copy(AST t) {
    AST *ret = (AST *)malloc(sizeof(AST));
    if (ret == NULL) {
	bail_with_error("Cannot allocate an AST heap copy!");
    }
    *ret = t;
    return ret;
}

// Return an AST for a program,
// which contains the given ASTs.
program_t ast_program(text_section_t textSec, data_section_t dataSec, stack_section_t stackSec)
{
    program_t ret;
    ret.file_loc = textSec.file_loc;
    ret.type_tag = program_ast;
    ret.textSection = textSec;
    ret.dataSection = dataSec;
    ret.stackSection = stackSec;
    return ret;
}

// Return an AST for the text section
// with the given entry point and instructions.
text_section_t ast_text_section(token_t tok, lora_t entryPoint,
				asm_instrs_t instrs)
{
    text_section_t ret;
    ret.file_loc = file_location_copy(tok.file_loc);
    ret.type_tag = text_section_ast;
    ret.entryPoint = entryPoint;
    ret.instrs = instrs;
    return ret;
}

// Return an AST for an entry point
// that represents the label given
lora_t ast_lora_label(ident_t label)
{
    lora_t ret;
    ret.file_loc = file_location_copy(label.file_loc);
    ret.type_tag = lora_ast;
    ret.address_defined = false;
    ret.label = label.name;
    ret.addr = 0;
    return ret;
}

// Return an AST for an entry point
// that represents the address given
lora_t ast_lora_addr(unsignednum_t addr)
{
    lora_t ret;
    ret.file_loc = file_location_copy(addr.file_loc);
    ret.type_tag = lora_ast;
    ret.address_defined = true;
    ret.label = NULL;
    ret.addr = addr.value;
    return ret;
}


// Return an AST for an asm instr AST
// with the given label-opt and instruction
asm_instr_t ast_asm_instr(label_opt_t labelOpt, instr_t instr)
{
    asm_instr_t ret;
    ret.file_loc = labelOpt.file_loc;
    ret.type_tag = asm_instr_ast;
    ret.next = NULL;
    ret.label_opt = labelOpt;
    ret.instr = instr;
    return ret;
}

// Return an AST for a singleton asm instrs AST
// with the given instruction
asm_instrs_t ast_asm_instrs_singleton(asm_instr_t asminstr)
{
    asm_instrs_t ret;
    ret.file_loc = asminstr.file_loc;
    ret.type_tag = asm_instrs_ast;
    asm_instr_t *p = (asm_instr_t *)malloc(sizeof(asm_instr_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for an asm_instr!");
    }
    *p = asminstr;
    p->next = NULL;
    ret.instrs = p;
    return ret;
}

// Return an AST made from adding instr to the end of lst
asm_instrs_t ast_asm_instrs_add(asm_instrs_t lst, asm_instr_t asminstr)
{
    asm_instrs_t ret = lst;
    asm_instr_t *p = (asm_instr_t *)malloc(sizeof(asm_instr_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for an asm_instr!");
    }
    *p = asminstr;
    p->next = NULL;
    // splice p onto the end of lst.instrs
    asm_instr_t *last = ast_last_list_elem(lst.instrs);
    if (last == NULL) {
	ret.instrs = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a label_opt AST
// with the given identifier
label_opt_t ast_label_opt_label(ident_t id)
{
    label_opt_t ret;
    ret.file_loc = file_location_copy(id.file_loc);
    ret.type_tag = label_opt_ast;
    ret.name = id.name;
    return ret;
}

// Return an AST for a label_opt AST
// with the empty AST (t)
label_opt_t ast_label_opt_empty(empty_t t)
{
    label_opt_t ret;
    ret.file_loc = file_location_copy(t.file_loc);
    ret.type_tag = label_opt_ast;
    ret.name = NULL;
    return ret;
}

// Return an immediate data holding an int
immedData_t ast_immed_number(int n) {
    immedData_t ret;
    ret.id_data_kind = id_number;
    ret.data.immed = n;
    return ret;
}

// Return an immediate data holding an unsigned int
immedData_t ast_immed_unsigned(unsigned int u) {
    immedData_t ret;
    ret.id_data_kind = id_unsigned;
    ret.data.uimmed = u;
    return ret;
}

// Return an immediate data that is nothing
immedData_t ast_immed_none() {
    immedData_t ret;
    ret.id_data_kind = id_empty;
    ret.data.none = 0;
    return ret;
}

// Return an immediate data holding an address
immedData_t ast_immed_lora(lora_t a) {
    immedData_t ret;
    ret.id_data_kind = id_lora;
    ret.data.lora = a;
    return ret;
}

// Return an AST for an instruction
// with the given information
extern instr_t ast_instr(token_t op, instr_type itype,
			 unsigned short num_regs_used,
			 unsigned short reg1, unsigned short reg2,
			 unsigned short reg3, func_type func,
			 immed_kind_t ik, immedData_t im)
{
    instr_t ret;
    ret.file_loc = file_location_copy(op.file_loc);
    ret.type_tag = instr_ast;
    ret.itype = itype;
    ret.opcode = lexer_token2opcode(op.code);
    ret.opname = op.text;
    ret.func = func;
    ret.regs_used = num_regs_used;
    ret.regs[0] = reg1;
    ret.regs[1] = reg2;
    ret.regs[2] = reg3;
    ret.immed_kind = ik;
    ret.immed_data = im;
    return ret;
}

// Return an AST for the data section AST
// with the given list of static declarations.
data_section_t ast_data_section(token_t kw, unsigned int static_start,
				static_decls_t staticDecls)
{
    data_section_t ret;
    ret.file_loc = file_location_copy(kw.file_loc);
    ret.type_tag = data_section_ast;
    ret.static_start_addr = static_start;
    ret.staticDecls = staticDecls;
    return ret;
}

// Return an AST for an empty list of static decls
static_decls_t ast_static_decls_empty(empty_t e)
{
    static_decls_t ret;
    ret.file_loc = file_location_copy(e.file_loc);
    ret.type_tag = static_decls_ast;
    ret.decls = NULL;
    return ret;
}

// Return an AST for a list of static declarations
// with sd added to the end of sds
static_decls_t ast_static_decls_add(static_decls_t sds,
				    static_decl_t sd)
{
    static_decls_t ret = sds;
    static_decl_t *p = (static_decl_t *)malloc(sizeof(static_decl_t));
    if (p == NULL) {
	bail_with_error("Cannot allocate space for a static_decl!");
    }
    *p = sd;
    p->next = NULL;
    // splice p onto the end of sds.decls
    static_decl_t *last = ast_last_list_elem(sds.decls);
    if (last == NULL) {
	ret.decls = p;
    } else {
	last->next = p;
    }
    return ret;
}

// Return an AST for a data size delclarator
// for the given number of bytes
data_size_t ast_data_size(token_t kw, unsigned short bytes)
{
    // duplicated from string.h to suppress a (silly) warning
    extern char *strdup(const char *s);
    data_size_t ret;
    ret.file_loc = file_location_copy(kw.file_loc);
    ret.type_tag = data_size_ast;
    ret.size_in_bytes = bytes;
    ret.size_name = strdup(kw.text);
    return ret;
}

// Return an AST for a static declaration
// found in the file named fn, on line ln, with the given data size,
// identifier, and initializer.
static_decl_t ast_static_decl(data_size_t ds,
			      ident_t ident,
			      initializer_t initializer)
{
    static_decl_t ret;
    ret.file_loc = file_location_copy(ds.file_loc);
    ret.type_tag = static_decl_ast;
    ret.size_in_bytes = ds.size_in_bytes;
    ret.size_name = ds.size_name;
    ret.ident = ident;
    ret.initializer = initializer;
    return ret;
}

// Return an AST for an initializer with the given value
initializer_t ast_initializer_given(token_t eqs, word_type value)
{
    initializer_t ret;
    ret.file_loc = file_location_copy(eqs.file_loc);
    ret.type_tag = initializer_ast;
    ret.number = value;
    return ret;
}

// Return an AST for a stack section AST
// with the given keyword and stack bottom address.
stack_section_t ast_stack_section(token_t kw, unsigned int stack_bottom)
{
    stack_section_t ret;
    ret.file_loc = file_location_copy(kw.file_loc);
    ret.type_tag = stack_section_ast;
    ret.stack_bottom_addr = stack_bottom;
    return ret;
}

// Return an AST for an empty initializer 
extern initializer_t ast_initializer_empty(empty_t e)
{
    initializer_t ret;
    ret.file_loc = file_location_copy(e.file_loc);
    ret.type_tag = initializer_ast;
    ret.number = 0;
    return ret;
}

// Return an AST for empty found in the file named fn, on line ln
empty_t ast_empty(const char *fn, unsigned int ln)
{
    empty_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.type_tag = empty_ast;
    return ret;
}

// Return an AST for an identifier
// found in the file named fn, on line ln, with the given name.
ident_t ast_ident(const char *fn, unsigned int ln, const char *name)
{
    ident_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.type_tag = ident_ast;
    ret.name = name;
    return ret;
}

// Return an AST for a (signed) number with the given value
number_t ast_number(token_t sgn, word_type value)
{
    number_t ret;
    ret.file_loc = file_location_copy(sgn.file_loc);
    ret.type_tag = number_ast;
    ret.value = value;
    return ret;
}

// Return an AST for an (unsigned) number
// with the given value
unsignednum_t ast_unsignednum(const char *fn, unsigned int ln,
			      unsigned int value)
{
    unsignednum_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.type_tag = unsignednum_ast;
    ret.value = value;
    return ret;
}

// Return an AST for a token
token_t ast_token(const char *fn, unsigned int ln, int token_code)
{
    token_t ret;
    ret.file_loc = file_location_make(fn, ln);
    ret.type_tag = token_ast;
    ret.code = token_code;
    return ret;
}    

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return a pointer to the last element in lst.
// This only returns NULL if lst == NULL.
void *ast_last_list_elem(void *lst)
{
    if (lst == NULL) {
	return lst;
    }
    // assert lst is not NULL
    void *prev = NULL;
    while (lst != NULL) {
	prev = lst;
	lst = ((generic_t *)lst)->next;
    }
    // here lst == NULL;
    return prev;
}

// Requires: lst is a pointer to a non-circular 
//           linked list with next pointers
//           as in generic_t
// Return the number of elements in the linked list lst
extern int ast_list_length(void *lst)
{
    int ret = 0;
    generic_t *p = (generic_t *) lst;
    while (p != NULL) {
	p = p->next;
	ret++;
    }
    return ret;
}
