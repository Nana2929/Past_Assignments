.globl __start # emit symbol_name to symbol table (scope GLOBAL)

.text
__start:
    # calling convention reference: https://smist08.wordpress.com/2019/09/07/risc-v-assembly-language-hello-world/
    # Read first operand
    li a0, 5  
    ecall
    mv s0, a0          # move function arg from s0(fp) to a0 
    jal Fib
    beq zero, zero, output
    j exit  

Fib:  
    slti t0, a0, 2
    beq zero, t0, Rec
    add s3, zero, a0    # output for base cases
    jr ra

Rec: # label jumped to for n>=2
    addi sp, sp, -12    # the stack pointer make 3 rooms
    sw ra, 8(sp)        # save return addr 
    sw a0, 4(sp)        # save n
  
    addi a0, a0, -1     # n-1
    jal Fib             # Calc F(n-1)
    lw a0, 4(sp)        # load back n 
    sw s3, 0(sp)        # save F(n-1) to stack 
    
    addi a0, a0, -2     # n-2 
    jal Fib             # Calc F(n-2)
    lw t5, 0(sp)        # load the stored F(n-1)
    add s3, s3, t5      # Fn = Fn-1 + Fn-2
    
    lw ra, 8(sp)        # return addr
    addi sp, sp, 12     # increment sp 
    jr ra 

output:
    # Output the result
    li a0, 1
    mv a1, s3
    ecall

exit:
    # Exit program(necessary)
    li a0, 10
    ecall