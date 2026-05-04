.text
.globl main
main:
li $t0, 12
sw $t0, i
lw $t0, i
li $t1, 10
add $t2, $t0, $t1
sw $t2, i
li $t0, 99
sw $t0, c
