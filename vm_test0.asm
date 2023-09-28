	# $Id: vm_test0.asm,v 1.1 2023/09/18 03:32:18 leavens Exp $
	.text start
start:	STRA
	ADDI $0, $t0, 1
	EXIT
	.data 1024
	.stack 4096
	.end
	
