# $Id: Makefile,v 1.32 2023/09/22 20:16:39 leavens Exp $
# Makefile for PL/0 compiler and code generation

# Add .exe to the end of target to get that suffix in the rules
VM = vm
CC = gcc
# on Linux, the following can be used with gcc:
# CFLAGS = -fsanitize=address -static-libasan -g -std=c17 -Wall
CFLAGS = -g -std=c17 -Wall
MV = mv
RM = rm -f
SUBMISSIONZIPFILE = submission.zip
ZIP = zip -9
# Add the names of your own files with a .o suffix to link them into the VM
VM_OBJECTS = machine_main.o machine.o \
             machine_types.o instruction.o bof.o \
             regname.o utilities.o 
SOURCESLIST = `echo $(VM_OBJECTS) | sed -e 's/\\.o/.c/g'`
TESTSOURCES = vm_test0.asm vm_test1.asm vm_test2.asm \
		vm_test3.asm vm_test4.asm vm_test5.asm vm_test6.asm \
		vm_test7.asm
TESTS = vm_test0.bof vm_test1.bof vm_test2.bof vm_test3.bof \
	vm_test4.bof vm_test5.bof vm_test6.bof vm_test7.bof
EXPECTEDOUTPUTS = $(TESTS:.bof=.out)
EXPECTEDLISTINGS = $(TESTS:.bof=.lst)
# STUDENTESTOUTPUTS is all of the .myo files corresponding to the tests
# if you add more tests, you can add more to this list,
# or just add to TESTS above
STUDENTTESTOUTPUTS = $(TESTS:.bof=.myo)
# STUDENTESTLISTINGS is all of the .myp files corresponding to the tests
# if you add more tests, you can add more to this list,
# or just add to TESTS above
STUDENTTESTLISTINGS = $(TESTS:.bof=.myp)

# create the VM executable
.PRECIOUS: $(VM)

$(VM): $(VM_OBJECTS)
	$(CC) $(CFLAGS) -o $(VM) $(VM_OBJECTS)

# rule for compiling individual .c files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) *~ *.o *.myo *.myp '#'*
	$(RM) $(VM).exe $(VM)
	$(RM) *.stackdump core
	$(RM) $(SUBMISSIONZIPFILE)

# rule for making .bof files with the assembler ($(ASM));
# this might need to be done if not running on Linux (or Windows)

%.bof: %.asm $(ASM)
	./$(ASM) $<

# Rules for making individual outputs (e.g., execute make test1.myo)
# the .myo files are outputs from running the .bof files in the VM
.PRECIOUS: %.myo %.myp
%.myo: %.bof $(VM)
	./$(VM) $< > $@ 2>&1

%.myp: %.bof $(VM)
	./$(VM) -p $< > $@ 2>&1

# main target for testing
.PHONY: check-outputs
check-outputs: $(VM) $(ASM) $(TESTS) check-lst-outputs check-vm-outputs 
	@echo 'Be sure to look for two test summaries above (listings and execution)'

check-lst-outputs check-asm-outputs:
	DIFFS=0; \
	for f in `echo $(TESTS) | sed -e 's/\\.bof//g'`; \
	do \
		echo listing "$$f.bof" using ./vm -p ...; \
		./vm -p "$$f.bof" > "$$f.myp" 2>&1; \
		diff -w -B "$$f.lst" "$$f.myp" && echo 'passed!' \
			|| { echo 'failed!'; DIFFS=1; }; \
	done; \
	if test 0 = $$DIFFS; \
	then \
		echo 'All listing tests passed!'; \
	else \
		echo 'Some listing test(s) failed!'; \
	fi

check-vm-outputs:
	DIFFS=0; \
	for f in `echo $(TESTS) | sed -e 's/\\.bof//g'`; \
	do \
		echo running "$$f.bof" in the VM ...; \
		./vm "$$f.bof" > "$$f.myo" 2>&1; \
		diff -w -B "$$f.out" "$$f.myo" && echo 'passed!' \
			|| { echo 'failed!'; DIFFS=1; }; \
	done; \
	if test 0 = $$DIFFS; \
	then \
		echo 'All VM execution tests passed!'; \
	else \
		echo 'Some VM execution test(s) failed!'; \
	fi

# Automatically generate the submission zip file
$(SUBMISSIONZIPFILE): *.c *.h $(STUDENTTESTOUTPUTS) $(STUDENTTESTLISTINGS) \
		Makefile 
	$(ZIP) $@ $^ asm.y asm_lexer.l $(EXPECTEDOUTPUTS) $(EXPECTEDLISTINGS)

# instructor's section below...

ASM = asm
DISASM = disasm
LEX = flex
LEXFLAGS =
YACC = bison
YACCFLAGS = -Wall --locations -d -v
LEXER = lexer

$(ASM)_lexer.c: $(ASM)_lexer.l
	$(LEX) $(LEXFLAGS) $<

$(ASM)_lexer.o: $(ASM)_lexer.c ast.h $(ASM).tab.h utilities.h file_location.h
	$(CC) $(CFLAGS) -Wno-unused-but-set-variable -Wno-unused-function -c $<

$(ASM).tab.o: $(ASM).tab.c $(ASM).tab.h
	$(CC) $(CFLAGS) -Wno-unused-const-variable -c $<

$(ASM).tab.c $(ASM).tab.h: $(ASM).y ast.h parser_types.h machine_types.h 
	$(YACC) $(YACCFLAGS) $(ASM).y

lexer.o: lexer.c lexer.h $(ASM).tab.h
	$(CC) $(CFLAGS) -c $<

$(LEXER) : $(LEXER)_main.o $(LEXER).o $(ASM)_lexer.o ast.o $(ASM).tab.o file_location.o lexer.o utilities.o 
	$(CC) $(CFLAGS) $^ -o $@

$(ASM)_main.o: $(ASM)_main.c $(ASM).tab.h ast.h parser_types.h machine_types.h

$(ASM): $(ASM)_main.o $(ASM).tab.o $(ASM)_lexer.o $(ASM)_unparser.o ast.o bof.o file_location.o lexer.o pass1.o assemble.o instruction.o machine_types.o regname.o symtab.o utilities.o
	$(CC) $(CFLAGS) $^ -o $@

$(DISASM): disasm_main.o disasm.o instruction.o bof.o machine_types.o regname.o utilities.o
	$(CC) $(CFLAGS) -o $(DISASM) $^

.PRECIOUS: %.out %.lst
%.out: %.bof $(VM)
	@if test '$(IMTHEINSTRUCTOR)' != true ; \
	then \
		echo 'Students should NOT use the target $@,'; \
		echo 'as using this target ($@) will invalidate a test'; \
		exit 1; \
	fi
	$(RM) $@
	./$(VM) $< > $@ 2>&1

%.lst: %.bof $(VM)
	@if test '$(IMTHEINSTRUCTOR)' != true ; \
	then \
		echo 'Students should NOT use the target $@,'; \
		echo 'as using this target ($@) will invalidate a test'; \
		exit 1; \
	fi
	./$(VM) -p $< > $@ 2>&1

.PHONY: create-outputs
create-outputs: $(VM) $(ASM) $(TESTS)
	@if test '$(IMTHEINSTRUCTOR)' != true ; \
	then \
		echo 'Students should use the target check-outputs,' ;\
		echo 'as using this target (create-outputs) will invalidate the tests!' ; \
		exit 1; \
	fi
	$(MAKE) create-vm-outputs
	$(MAKE) create-asm-outputs

create-vm-outputs: $(VM) $(ASM) $(TESTS)
	@if test '$(IMTHEINSTRUCTOR)' != true ; \
	then \
		echo 'Students should not use the target create-vm-outputs,'; \
		echo 'as using this target will invalidate the tests!' ; \
		exit 1; \
	fi
	for f in `echo $(TESTS) | sed -e 's/\\.bof//g'`; \
	do \
		$(RM) "$$f.out"; \
		echo running "$$f.bof" " ... in the VM"; \
		./$(VM) "$$f.bof" > "$$f.out" 2>&1; \
	done
	echo 'done creating VM test tracing outputs!'

create-asm-outputs: $(VM) $(ASM) $(TESTS)
	@if test '$(IMTHEINSTRUCTOR)' != true ; \
	then \
		echo 'Students should not use the target create-vm-outputs,'; \
		echo 'as using this target will invalidate the tests!' ; \
		exit 1; \
	fi
	for f in `echo $(TESTS) | sed -e 's/\\.bof//g'`; \
	do \
		$(RM) "$$f.lst"; \
		./$(VM) -p "$$f.bof" > "$$f.lst" 2>&1; \
	done
	echo 'done creating listing outputs!'

.PHONY: digest
digest digest.txt: $(EXPECTEDOUTPUTS) $(EXPECTEDLISTINGS)
	for f in $(EXPECTEDLISTINGS) ; \
        do cat $$f; echo " "; \
	cat `echo $$f | sed -e 's/\\.lst/.out/'`; \
        echo " "; echo " "; \
        done >digest.txt

# don't use develop-clean unless you want to regenerate the expected outputs
.PHONY: develop-clean bof-clean asm-clean
develop-clean: clean asm-clean
	$(RM) digest.txt
	$(RM) y.tab.h

asm-clean:
	$(RM) $(ASM)_lexer.[ch] $(ASM).tab.[ch] asm.output
	$(RM) $(ASM).exe $(ASM) $(DISASM).exe $(DISASM) $(LEXER) $(LEXER).exe

outputs-clean: clean asm-clean bof-clean
	$(RM) $(EXPECTEDOUTPUTS) $(EXPECTEDLISTINGS)

bof-clean:
	$(RM) *.bof

TESTSZIPFILE = ~/temp/hw1-tests.zip
PROVIDEDFILES = Makefile asm_main.c asm.y asm_lexer.l \
		asm_lexer.c asm.tab.h parser_types.h \
		asm_unparser.[ch] ast.[ch] bof.[ch] machine_types.[ch] \
		utilities.[ch] file_location.[ch] lexer.[ch] \
		pass1.[ch] assemble.[ch] instruction.[ch] regname.[ch] \
		symtab.[ch] utilities.[ch] id_attrs.h \
		disasm_main.c disasm.[ch] \
		vm_test*.asm vm_test*.out vm_test*.bof vm_test*.lst

.PHONY: zip
zip hw1-tests.zip: create-vm-outputs $(TESTSZIPFILE)

$(TESTSZIPFILE): Makefile $(TESTS) $(PROVIDEDFILES)
	$(RM) $(TESTSZIPFILE)
	chmod 444 Makefile $(TESTS) $(PROVIDEDFILES)
	$(ZIP) $(TESTSZIPFILE) Makefile $(TESTS) $(PROVIDEDFILES)
	chmod 754 Makefile $(TESTS) $(PROVIDEDFILES)

%.bof: %.$(ASM)
	./$(ASM) $<

.PHONY: check-separately
check-separately:
	$(CC) $(CFLAGS) -c *.c

.PHONY: hw4-solution.zip
hw4-solution.zip ~/temp/hw4-solution.zip: Makefile $(TESTS) $(PROVIDEDFILES) \
			*.pl0 *.vmo *.vmi vm/*.[ch] vm/Makefile \
			proc_holder.[ch] idUseCheck.[ch] \
			sources.txt vm/sources.txt
	$(MAKE) clean
	$(ZIP) ~/temp/hw4-solution.zip $^
