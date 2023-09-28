	# $Id: vm_test5.asm,v 1.1 2023/09/19 06:57:49 leavens Exp $
	.text start
start:	ADDI $0, $t0, 0x7 # $t0 is 7
	ADDI $0, $t1, 0xf # $t1 is 15
	LW   $gp, $t2, 0  # $t2 should be 2
	AND  $t0, $t2, $t3 # $t3 should be 2
	SLL  $t3, $t4, 5  # $t4 should be 64
	ADDI $0, $t5, 1   # $t5 is 1
	SLL $t5, $t5, 7   # $t5 is 128
	SRL $t5, $t5, 1   # $t5 is 64
	BNE $t3, $t5, 2
	LBU $gp, $a0, 1   # put N in $a0
        JAL outch
        XOR $t4, $t5, $t6 # $t6 should be 0
	BGTZ $t6, 2
	LBU $gp, $a0, 1   # put N in $a0
        JAL outch
	BGTZ $t6, 2
	LBU $gp, $a0, 3   # put H in $a0
        JAL outch
	BLTZ $t6, 2
	LBU $gp, $a0, 2   # put L in $a0
        JAL outch
        BEQ $0, $t6, 2    # is $t6 0?
        LW $gp, $a0, 5    # put Y in $a0
        JAL outch
        BOR $t5, $t4, $t7 # put 65 in $t7
        ADDI $0, $t8, 65  # put 65 in $t8
	NOR $t5, $t3, $t8 # $t8 is 0xffffffbc
	ADDI $0, $t9, 0xffffffbc  # $t9 is 0xffffffbc
	XORI $t9, $v0, 0x20 # $v0 is 0xffffff9c
        XORI $v0, $v1, 0xffffff9c
	BEQ $0, $v1, 2
	LW $gp, $a0, 5   # $a0 is Y
	JAL outch
	EXIT

outch:	NOTR
	PCH               # arg already in $a0
newl:	ADDI $0, $a0, 10  # \n
	PCH               
	# STRA
	JR $ra
	
	.data 512
	WORD TWO = 2
	WORD N = 78
	WORD L = 76
	WORD H = 72
	WORD nl = 10
	WORD Y = 89
	.stack 4096
	.end
	
