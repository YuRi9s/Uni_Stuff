	.data
	.globl modulo_str
	.text

# --- modulo_str ---
# Arguments:
# a0: start address of the buffer
# a1: number of bytes in the buffer
# a2: divisor
# Return:
# v0: the decimal number (encoded using ASCII digits '0' to '9') in the buffer [$a0 to $a0 + $a1 - 1] modulo $a2 
#Note that $t1 is for the least sig numb and $t1 for the last numb in the buffer $t3 is for the current position 
modulo_str:
	# TODO 
	li $t2 0
	move $t0 $a0

loop:

	lb $t3 ($t0)
	beqz $a1 end
	subiu $a1 $a1 1
	sub $t3 $t3 48 # to store the ASCII code in $t3
	mul $t2 $t2 10
	add $t2 $t2 $t3
	div $t2 $t2 $a2
	mfhi $t2 #set the $t2 to the remainder
	add $t0 $t0 1 # move  to the next digit
	j loop

end:
	move $v0 $t2
	jr	$ra
		
