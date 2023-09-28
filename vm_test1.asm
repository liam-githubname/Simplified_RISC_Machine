	# $Id: vm_test1.asm,v 1.3 2023/09/22 20:16:39 leavens Exp $
	.text start
start:	STRA
	ADDI $0, $t0, 1   # $t0 is 1
	ADD $t0, $t0, $t2 # $t2 is 2
	ADD $t2, $t0, $t0 # $t0 is 3
	ADD $t0, $t0, $t0 # $t0 is 6
	ADD $t0, $t0, $t0 # $t0 is 12
	SUB $t0, $t2, $t3 # $t3 is 10 ('\n')
	ADDI $0, $t1, 89  # $t1 is 89 ('Y')
	NOTR
	ADD $0, $t1, $a0  # move 89 to $a0
	PCH
	ADD $0, $t3, $a0  # move 10 to $a0
	PCH
	EXIT
	.data 1024
	.stack 4096
	.end
	
