.globl __start # emit symbol_name to symbol table (scope GLOBAL)
.data
    seqlen: .word 0 
    .align 4 
    seq: .space 200 # base address of seq 
.text
__start:
    # calling convention reference: 
    # https://smist08.wordpress.com/2019/09/07/risc-v-assembly-language-hello-world/
    # Read first operand
    li a0, 5  
    ecall
    mv s0, a0          # move function arg from s0(fp) to a0
    jal Recaman
    beq zero, zero, output 
    j exit 
    
Recaman: 
    # base step: n=0
    
    addi s3, a0, 0    # res = a
    addi sp, sp, -8   # make rooms
    sw ra, 4(sp)      # save ra 
    sw a0, 0(sp)      # save n     
    beq a0, zero, add_to_seq 
    
    # recursive steps: 
    bne a0, zero, next
    
    # back to line 14 
    jr ra             
    
next:    
    ############
    addi a0, a0, -1    
    jal Recaman       # return value stored at s3 
    ##############
    # load back return address 
    # (ra is saved when recursive calls are required)
    
    lw ra, 4(sp)      # load return address 
    lw a0, 0(sp)      # load n 
    sub t5, s3, a0    # a: t5 = t - n 
    add t6, s3, a0    # b: t6 = t + n
    # if a > 0 
    bgt t5, zero, CHECK # branch if t5 > zero
    ble t5, zero,  else # branch if t5 <= zero
    # load return address
    jr ra 
else:
    add s3, t6, zero   # s3 = b 
    jal add_to_seq
    jr ra
    
CHECK:
    li s7, 0           # i = 0 
  LOOP:
    la s4, seq         # s4: base addr of seq  
    slli s10, s7, 2    # 4 bytes i
    add s10, s10, s4   
    lw s5, 0(s10)      # seq[i]
    # note: return address 
    beq s5, t5, LABEL  # if seq[i] == a
    addi s7, s7, 1     # i+=1
    lw t2, seqlen      # load seqlen as value    
    blt s7, t2, LOOP   
    # end of loop, no LABEL 
    add s3, t5, zero   # s3 = a 
    jal add_to_seq
    jr ra
  SAME: 
    # LABEL
    add s3, t6, zero   # s3 = b
    jr ra   

LABEL: 
    add s3, t6, zero   # s3 <- b 
    jal add_to_seq  
    jal SAME 
    
add_to_seq:  
    la t1, seq       # t1: seq base address 
    
    #bug?
    lw t2, seqlen    # t2: seqlen as value
    
    # bug?
    la t3, seqlen    # t3: seqlen's address
    ########
    addi t4, t2, 0   # copy seqlen value   
    slli t2, t2, 2   # (seqlen)*4 
    add t1, t1, t2   # t1: seq[seqlen]'s address 
    sw s3, 0(t1)     # res <- seq[seqlen] 
    addi t4, t4, 1   
    sw t4, 0(t3)     # seqlen+=1 
    lw ra, 4(sp) 
    addi sp, sp, 8 
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