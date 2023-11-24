	.data
	.globl iban2knr
	.text
# -- iban2knr
# Arguments:
# a0: IBAN buffer (22 bytes)
# a1: BLZ buffer (8 bytes)
# a2: KNR buffer (10 bytes)
# t3 is the counter that I add 
# t1 for BLZ
# t2 for KNR
iban2knr:
	# TODO
	move $t0 $a0
	move $t1 $a1
	move $t2 $a2
	add $t0 $t0 4
	li $t3 0

loop:
	lb $t4 ($t0)
	beqz $t4 end
	add $t0 $t0 1
	add $t3 $t3 1

BLZ:
	bgt $t3 8 KNR
	sb $t4 ($t1)
	add $t1 $t1 1
	j loop
		
KNR:
	bgt $t3 18 end	
	sb $t4 ($t2)
	add $t2 $t2 1
	j loop
		

end:
	jr	$ra
