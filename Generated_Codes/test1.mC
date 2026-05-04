.text
.globl main
main:
li $t0, 12
sw $t0, i
lw $t1, i
li $t2, 10
add $t3, $t1, $t2
sw $t3, i
li $t4, 99
sw $t4, c
