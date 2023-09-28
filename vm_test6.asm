	# $Id: vm_test6.asm,v 1.2 2023/09/21 18:48:31 leavens Exp $
	.text start
start:	ADDI $0, $t0, 1	# $t0 is 1
	SLL $t0, $t1, 20  # $t1 is 2^20
	ADD $t0, $t1, $t2 # $t2 is 2^20+1
	ADD $t0, $t0, $t3 # $t3 is 2
	DIV $t2, $t3      # HI is 1, LO is 524288
	MFLO $t4          # $t4 is 524288
	MFHI $t5          # $t5 is 1
	SW $gp, $t5, 0    # put $t5 into rem
	SW $gp, $t4, 1    # put $t4 into dividend
	SLL $t0, $t6, 10  # put 2^10 (i.e., 1024) in $t6
	MUL $t6, $t2
	MFHI $t7
	ADDI $sp, $sp, -4  # allocate space on the stack
	SW   $sp, $t7, 0  # store $t7 on the stack
	EXIT
	.data 1024
	WORD rem = 0
	WORD dividend = 0
	.stack 4096
	.end
