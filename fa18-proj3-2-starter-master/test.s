	addi a0 x0 5
	addi t0 x0 0
	addi s0 x0 1
start: 
	beq a0 x0 end
	addi t0 t0 1
	mul s0 s0 t0
	addi a0 a0 -1
	jal ra start
end:
	addi t1 x0 1
	add t1 t1 t1