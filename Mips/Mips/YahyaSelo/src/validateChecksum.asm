	.data
	.globl validate_checksum
	#there are 22 Characters and DE will change into 4 characters 
	array: .space 24
	.text

# -- validate_checksum --
# Arguments:
# a0 : Address of a string containing a german IBAN (22 characters)
# Return:
# v0 : the checksum of the IBAN
# t0 is where the numbers are stored without DE22
#t1 is ther counter 
validate_checksum:
	la $t0 4($a0)
	la $t3 array
	li $t1 0
loop:
	lb $t2 ($t0)
	sb $t2 ($t3)
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
	sb $t2 ($t3)
	mfhi $t2
	addiu $t2 $t2 48
	addiu $t3 $t3 1
	sb $t2 ($t3)
	addiu $t0 $t0 1
	addiu $t3 $t3 1
	addiu $t1 $t1 1
	blt $t1 2 loopChar
	#reset the counter again
	li $t1 0
loopNumb:
	lb $t2 ($t0)
	sb $t2 ($t3)
	addiu $t1 $t1 1
	addiu $t0 $t0 1
	addiu $t3 $t3 1
	blt $t1 2 loopNumb
	la $a0 array
	li $a1 24
	li $a2 97
	move $t4 $ra
	jal modulo_str
	move $ra $t4
	jr	$ra
