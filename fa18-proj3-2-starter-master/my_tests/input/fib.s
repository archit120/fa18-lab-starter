	addi a0 x0 3
	addi t0 x0 0
	addi t1 x0 1
loop:
	add s0 t1 t0
	add t1 t0 x0
	add t0 s0 x0
	addi a0 a0 -1
	bne a0 x0 loop