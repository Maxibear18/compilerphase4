.text
.globl main
main:
li $t0, 10
li $t1, 15
slt $t2, $t0, $t1
beq $t2, $zero, L0
li $t3, 0
sw $t3, i
j L1
L0:
li $t0, 5
sw $t0, i
L1:
