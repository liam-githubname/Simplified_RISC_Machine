	# $Id: vm_test7.asm,v 1.3 2023/09/22 20:16:39 leavens Exp $
	# manipulation of the runtime stack
	.text main
main:   ADDI $0, $at, 1	# put numbers in all the registers
	ADDI $0, $v0, 2
	ADDI $0, $v1, 3
	ADDI $0, $a0, 4
	ADDI $0, $a1, 5
	ADDI $0, $a2, 6
	ADDI $0, $a3, 7
	ADDI $0, $t0, 8
	ADDI $0, $t1, 9
	ADDI $0, $t2, 10
	ADDI $0, $t3, 11
	ADDI $0, $t4, 12
	ADDI $0, $t5, 13
	ADDI $0, $t6, 14
	ADDI $0, $t7, 15
	ADDI $0, $s0, 16
	ADDI $0, $s1, 17
	ADDI $0, $s2, 18
	ADDI $0, $s3, 19
	ADDI $0, $s4, 20
	ADDI $0, $s5, 21
	ADDI $0, $s6, 22
	ADDI $0, $s7, 23
	ADDI $0, $t8, 24
	ADDI $0, $t9, 25
	
        JAL sRegSave
	JAL callerSave
	JAL callerRestore
	JAL sRegRestore

	EXIT
	
	# callerSave is a subroutine that saves the registers
	# that a caller might need to save on the stack.
	# These registers are left undistubed...
	# However, since this is a subroutine, it can't save $ra,
	# and it disturbs $sp, so those need to be saved first
callerSave:	ADDI $sp, $sp, -4 # allocate 1 word on the stack
	SW $sp, $at, 0	   # save $at in the new space
	ADD $0, $sp, $at   # save current $sp in $at
	ADDI $sp, $sp, -64 # allocate 16 words on the stack
	SW $at, $v0, -1    # save $v0 in the next word on the stack
	SW $at, $v1, -2    # save $v1 in the next word on the stack
	SW $at, $a0, -3    # save $a0 in the next word on the stack
	SW $at, $a1, -4    # save $a1 in the next word on the stack	
	SW $at, $a2, -5    # save $a2 in the next word on the stack
	SW $at, $a3, -6    # save $a3 in the next word on the stack	
	SW $at, $t0, -7    # save $t0 in the next word on the stack
	SW $at, $t1, -8    # save $t1 in the next word on the stack
	SW $at, $t2, -9    # save $t2 in the next word on the stack
	SW $at, $t3, -10   # save $t3 in the next word on the stack	
	SW $at, $t4, -11   # save $t4 in the next word on the stack
	SW $at, $t5, -12   # save $t5 in the next word on the stack
	SW $at, $t6, -13   # save $t6 in the next word on the stack
	SW $at, $t7, -14   # save $t7 in the next word on the stack
	SW $at, $t8, -15   # save $t8 in the next word on the stack
	SW $at, $t9, -16   # save $t9 in the next word on the stack
	LW $at, $at, 0     # restore $at
        JR $ra
	
	
	# callerRestore is a subroutine that restores all registers
	# that were saved by callerSave (on the stack)
	# Assuming the runtime stack is in the same state it was
	# after calling callerSave, it also restores the $sp
	# to its previous value.
	# Note that the user will need to restore $ra manually
	# after calling this subroutine.
	
callerRestore: LW $sp, $t9, 0  # restore $t9...
	LW $sp, $t8, 1
	LW $sp, $t7, 2
	LW $sp, $t6, 3
	LW $sp, $t5, 4
	LW $sp, $t4, 5
	LW $sp, $t3, 6
	LW $sp, $t2, 7
	LW $sp, $t1, 8
	LW $sp, $t0, 9
	LW $sp, $a3, 10
	LW $sp, $a2, 11
	LW $sp, $a1, 12
	LW $sp, $a0, 13
	LW $sp, $v1, 14
	LW $sp, $v0, 15
	LW $sp, $at, 16
	ADDI $sp, $sp, 68  # deallocate 17 words
        JR $ra

	# Save the s registers ($s0-$s7) on the runtime stack.
	# Note that this changes $sp and calling it changes $ra
	# so those may need to be saved first.
sRegSave: ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s0, 0		# save $s0 register
	ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s1, 0		# save $s1 register
	ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s2, 0		# save $s2 register
	ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s3, 0		# save $s3 register
	ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s4, 0		# save $s4 register
	ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s5, 0		# save $s5 register
	ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s6, 0		# save $s6 register
	ADDI $sp, $sp, -4	# allocate 1 word on the stack
	SW $sp, $s7, 0		# save $s7 register	
	JR $ra
	
	# Restore the s registers ($s0-$s7) from the runtime stack.
	# Note that this changes $sp and calling it changes $ra
	# so those may need to be restored after calling this.
sRegRestore: LW $sp, $s7, 0	# restore the $s7 register	
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	LW $sp, $s6, 0		# restore the $s6 register
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	LW $sp, $s5, 0		# restore the $s5 register
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	LW $sp, $s4, 0		# restore the $s4 register
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	LW $sp, $s3, 0		# restore the $s3 register
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	LW $sp, $s2, 0		# restore the $s2 register
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	LW $sp, $s1, 0		# restore the $s1 register
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	LW $sp, $s0, 0		# restore the $s0 register
	ADDI $sp, $sp, 4	# deallocate 1 word on the stack
	JR $ra
	
	.data 1024
	.stack 4096
	.end
