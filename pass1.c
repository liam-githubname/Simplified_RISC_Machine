// $Id: pass1.c,v 1.2 2023/09/14 21:19:33 leavens Exp $
#include <stddef.h>
#include "pass1.h"
#include "symtab.h"
#include "utilities.h"

// Build the symbol table and check for duplicate declarations in the given AST
void pass1(program_t progast)
{
    symtab_initialize();
    pass1TextSection(progast.textSection);
    pass1DataSection(progast.dataSection);
}

// Check the given AST and put its declarations in the symbol table
void pass1TextSection(text_section_t ts)
{
    pass1AsmInstrs(ts.instrs);
}

// Check the given AST and put its declarations in the symbol table
void pass1AsmInstrs(asm_instrs_t instrs)
{
    asm_instr_t *ip = instrs.instrs;
    address_type count = 0;
    while (ip != NULL) {
	pass1AsmInstr(*ip, count);
	ip = ip->next;
	count++;
    }
}

// Check the given AST and put its declarations in the symbol table
void pass1AsmInstr(asm_instr_t instr, address_type count)
{
    pass1LabelOpt(instr.label_opt, count);
}

// Check the given AST and put its declarations in the symbol table
void pass1LabelOpt(label_opt_t lopt, address_type count)
{
    if (lopt.name != NULL) {
	id_attrs attrs;
	if (symtab_defined(lopt.name)) {
	    bail_with_error("Duplicate declaration of label \"%s\"",
			    lopt.name);
	}
	attrs.name = lopt.name;
	attrs.kind = id_label;
	attrs.addr = count;
	symtab_insert(attrs);
    }
}

// Check the given AST and put its declarations in the symbol table
void pass1DataSection(data_section_t ds)
{
    pass1StaticDecls(ds.staticDecls);
}

// Check the given AST and put its declarations in the symbol table
void pass1StaticDecls(static_decls_t sds)
{
    address_type offset = 0;
    static_decl_t *dcl = sds.decls;
    while (dcl != NULL) {
	pass1StaticDecl(*dcl, offset);
	offset = offset + dcl->size_in_bytes;
	dcl = dcl->next;
    }
}

// Check the given AST and put its declarations in the symbol table
void pass1StaticDecl(static_decl_t dcl, address_type offset)
{
    pass1Ident(dcl.ident, offset);
}

// Check the given AST and put its declarations in the symbol table
void pass1Ident(ident_t id, address_type offset)
{
    if (symtab_defined(id.name)) {
	bail_with_error("Duplicate declaration of data name \"%s\"",
			id.name);
    }
    id_attrs attrs;
    attrs.name = id.name;
    attrs.kind = id_data;
    attrs.addr = offset;
    symtab_insert(attrs);
}

// Print, on out, the header line for the printed output
// followed by a newline
static void pass1_print_header(FILE *out)
{
    fprintf(out, "%-5s %s\t%s\n", "Kind", "Name", "Address");
}

static const char *id_attr_kind_2_string(id_attr_kind k) {
    switch (k) {
    case id_label:
	return "Label";
	break;
    case id_data:
	return "Data";
	break;
    default:
	bail_with_error("Unknown id_attr_kind in id_attr_kind_2_string (%d)",
			k);
	break;
    }
    return NULL;
}

// Requires: symtab_defined(name);
// Print, on out, a line with the symbol table's information about name
// followed by a newline
static void pass1_print_name_info(FILE *out, const char *name)
{
    id_attrs na, *p;
    p = symtab_lookup(name);
    if (p == NULL) {
	bail_with_error("pass1_print_name_info given NULL pointer!");
    } else {
	na = *p;
    }
    fprintf(out, "%-5s %s\t%u\n", id_attr_kind_2_string(na.kind),
	    na.name, na.addr);
}

// Requires: out is a character file (like stdout)
//           that is open for writing
// Print debugging information about the symbol table on out
void pass1_print(FILE *out)
{
    pass1_print_header(out);
    const char *name = name = symtab_first_name();
    while (symtab_more_after(name)) {
	pass1_print_name_info(out, name);
	name = symtab_next_name(name);
    }
}
