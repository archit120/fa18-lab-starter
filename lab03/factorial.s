.data
n: .word 7

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial
    
    addi a1, a0, 0 
    addi a0, x0, 1
    ecall # Print Result
    
    addi a0, x0, 10
    ecall # Exit

factorial:
	addi t1, x0, 1
loop:
    beq a0, x0, exit
    mul t1, a0, t1
    addi a0, a0, -1
    j loop
exit:
	add a0, t1, x0
	jr ra
    # YOUR CODE HERE