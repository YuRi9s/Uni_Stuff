	.data
	.globl validate_checksum
	.text


# -- validate_checksum --
# Arguments:
# a0 : Address of a string containing a german IBAN (22 characters)
# Return:
# v0 : the checksum of the IBAN
validate_checksum:
	# TODO
	la $t0 4($a0)
	li $t1 0
	li $t5 0 # for the mod
loop:
	lb $t2 ($t0)
	addiu $t0 $t0 1
	addiu $t3 $t3 1
	addiu $t1 $t1 1
	blt $t1 18 loop
	#now to the first 4 characters 
	#reset the counter to 0
	li $t1 0
	la $t0 ($a0)
	
loopChar:
	lb $t2 ($t0)
	addi $t2 $t2 -55
	# div the D in DE into two characters change them to numbers and store both into two separate regesters 
	div $t2 $t2 10
	mflo $t2 
	addiu $t2 $t2 48
	mfhi $t2
	addiu $t2 $t2 48
	addiu $t3 $t3 1
	addiu $t0 $t0 1
	addiu $t3 $t3 1
	addiu $t1 $t1 1

	#sub $t3 $t3 48 # to store the ASCII code in $t3
	mul $t5 $t5 10
	add $t5 $t5 $t2
	div $t5 $t5 $a2
	mfhi $t5 #set the $t5 to the remainder

	blt $t1 2 loopChar
	#reset the counter again
	li $t1 0
	
loopNumb:
	lb $t2 ($t0)
	addiu $t1 $t1 1
	addiu $t0 $t0 1
	addiu $t3 $t3 1

	sub $t3 $t3 48 # to store the ASCII code in $t3
	mul $t2 $t2 10
	add $t2 $t2 $t3
	div $t2 $t2 $a2
	mfhi $t2 #set the $t2 to the remainder

	blt $t1 2 loopNumb
	la $a0 array
	li $a1 24
	li $a2 97


	jr	$ra