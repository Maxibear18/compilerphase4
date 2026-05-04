.text
.globl main
main:
li $t0, 2
li $t1, 3
li $t2, 5
mul $t3, $t1, $t2
li $t4, 7
div $t3, $t4
mflo $t5
add $t6, $t0, $t5
sw $t6, i
lw $t7, i
li $t8, 1
add $t9, $t7, $t8
sw $t9, i
