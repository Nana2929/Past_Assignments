.globl __start # emit symbol_name to symbol table (scope GLOBAL)

.rodata

    division_by_zero: .string "division by zero"
    remainder_by_zero: .string "remainder by zero"
    JumpTable: .word L0, L1, L2, L3, L4, L5, L6

.text
__start: # a label 
    # Read first operand
    li a0, 5 # load immediate 
    ecall
    mv s0, a0
    # Read operation
    li a0, 5
    ecall
    mv s1, a0 #  s1 stores op 
    # Read second operand
    li a0, 5
    ecall
    mv s2, a0
    # registers for use: https://en.wikipedia.org/wiki/RISC-V#Register_sets
    la  t3, JumpTable  # set t3 as the base addr of the JumpTable
    slli t4, s1, 2 # indexing using op # BY WORD(hence the "2" bytes)
    add t5, t4, t3 # final addr <- offset + base 
    lw t6, 0(t5) # from 0(t5) loads the calculated result
    jr t6 
      

# JumpTable
# each label is a one-word entry
L0:
    add s3, s0, s2 # use s3 to store the result
    beq zero, zero, output # jump to output (if zero == zero, which is conditional but must be true)
    j exit # pseudo instructuon for "jal x0, exit" # do we need to use this? 
L1:
    sub s3, s0, s2
    beq zero, zero, output 
    
L2:
    mul s3, s0, s2
    beq zero, zero, output 
    j exit 
L3: # division 
    div s3, s0, s2
    beq s2, zero, division_by_zero_except # if B == 0
    beq zero, zero, output
    j exit 
L4: # remainder
    rem s3, s0, s2
    beq s2, zero, remainder_by_zero_except # if B == 0
    beq zero, zero, output
    j exit 
L5: 
    # A^B 
    add t2, zero, zero # t2 stores k (counter)
    addi s3, zero, 1   # (init return value) s3 = 1 
    beq s2, zero, output  # if B is 0, output 1 
    loop: addi t2, t2, 1  # k increments 
          mul s3, s3, s0  # s3 *= A 
          beq t2, s2, output # if k == B, output 
          beq zero, zero, loop # else keep looping
    j exit        
L6:
    add t2, zero, zero # t2 = k
    addi s3, zero, 1   # s3 = 1
    slti t6, s0, 2     # set t6 = 1 if A < 2 (A= 1 or 0)
    bne t6, zero, output # if t6 != 0 (t6 = 1) output 1 as answer 
    loop2: addi t2, t2, 1  # k += 1
          mul s3, s3, t2   # s3*=k
          beq t2, s0, output # if k == A, output
          beq zero, zero, loop2 # else keep looping
    j exit 
           
output:
    # Output the result
    li a0, 1
    mv a1, s3
    ecall

exit:
    # Exit program(necessary)
    li a0, 10
    ecall

division_by_zero_except:
    li a0, 4
    la a1, division_by_zero
    ecall
    jal zero, exit

remainder_by_zero_except:
    li a0, 4
    la a1, remainder_by_zero
    ecall # invoke a system call 
    jal zero, exit 
    # jump and link: not wanting to store the return addr 
    # you can simply provide the zero register x0
