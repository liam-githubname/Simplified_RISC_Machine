	# $Id: vm_test3.asm,v 1.3 2023/09/18 16:03:54 leavens Exp $
	.text start
start:	STRA
	ADDI $0, $a0, 30  # $a0 is 30
	ADDI $0, $a1, 20  # $a1 is 20
	JAL sub
	ADDI $v0, $t0, 0   # $t0 should be 10
	ADDI $0, $t1, 10  # $t1 is 10
	BNE $t0, $t1, +8 # skip over if not 10
	JAL yes
	JMP done
	JAL no
	JMP done
yes:    NOTR
	ADDI $0, $a0, 89  # $a0 is 89 ('Y')
	PCH               # print Y
nl:	ADDI $0, $a0, 10  # $a0 is 10
	PCH
	STRA              # trace again
	JR $ra            # return (from both yes and no)
no:	NOTR
	ADDI $0, $a0, 78  # $a0 is 78 ('N')
	PCH
	JMP nl            # goto nl
sub:	SUB $a0, $a1, $v0
	JR $ra

done:	EXIT
	
	.data 1024
	WORD x = 33
	.stack 4096
	.end
	
