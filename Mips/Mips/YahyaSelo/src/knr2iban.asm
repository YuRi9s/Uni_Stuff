	.data
	.globl knr2iban
	.text
# -- knr2iban
# Arguments:
# a0: IBAN buffer (22 bytes)
# a1: BLZ buffer (8 bytes)
# a2: KNR buffer (10 bytes)
knr2iban:
	# TODO
	la $t0 ($a0)
	li $t1 68
	sb $t1 ($t0)
	li $t1 69
	sb $t1 1($t0)
	li $t1 48
	sb $t1 2($t0)
	sb $t1 3($t0)
	#conter 
	li $t1 0
	addiu $t0 $t0 4
loopBlz:
	lb $t2 ($a1)
	sb $t2 ($t0)
	addiu $t0 $t0 1
	addiu $t1 $t1 1
	addiu $a1 $a1 1
	blt $t1 8 loopBlz
	#conter 
	li $t1 0
loopKnr:
	lb $t2 ($a2)
	sb $t2 ($t0)
	addiu $t0 $t0 1
	addiu $t1 $t1 1
	addiu $a2 $a2 1
	blt $t1 10 loopKnr

	la $t7 ($a0)
	move $t6 $ra
	jal validate_checksum
	move $ra $t6

	la $a0 ($t7)
	# add 98 to a reg t0 because it already done its purpose 
	li $t0 98
	subu $t0 $t0 $v0
	la $t3 2($a0)
	div $t0 $t0 10
	mflo $t2 
	addiu $t2 $t2 48
	sb $t2 ($t3)
	mfhi $t2
	addiu $t2 $t2 48
	addiu $t3 $t3 1
	sb $t2 ($t3)
	jr	$ra
