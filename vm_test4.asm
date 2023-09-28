	# $Id: vm_test4.asm,v 1.1 2023/09/18 17:55:04 leavens Exp $
	.text start
start:	NOTR
	STRA
	ADDI $0, $t0, 5   # $t0 is 5
	ADDI $t0, $t1, 2  # $t1 should be 7
	ADD $t0, $t1, $t2  # $t2 should be 12
	MUL $t0, $t2      # LO should be 60
	MFLO $t3          # $t3 should be 60
	SUB $t1, $t0, $t4 # $t4 should be 2
	MUL $t3, $t4      # LO should be 120
	MFLO $t5          # $t5 should be 120
	DIV $t5, $t4	  # HI should be 0, LO s.b. 60
	MFLO $t6          # $t6 should be 60
	MFHI $s7          # $s7 should be 0
	BEQ $0, $s7, 8    # write N if $s7 is not 0
	ADDI $0, $a0, 78  # 'N'
        JAL outch
        BLTZ $s7, 8       # write L if $s7 < 0
	ADDI $0, $a0, 76  # 'L'
        JAL outch
        BGTZ $s7, 8       # write H if $s7 < 0
	ADDI $0, $a0, 72  # 'H'
        JAL outch
	ADDI $0, $a0, 89  # 'Y'
	JAL outch
	EXIT

outch:	NOTR
	PCH               # arg already in $a0
	ADDI $0, $a0, 10  # \n
	PCH               
	STRA
newl:	NOTR
	ADDI $0, $a0, 78  # $a0 is 78 ('N')
	PCH
	JR $ra
	
	.data 512
	WORD N = 78
	WORD L = 76
	WORD H = 72
	WORD nl = 10
	.stack 4096
	.end
	
