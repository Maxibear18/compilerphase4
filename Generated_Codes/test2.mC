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
lw $t0, i
li $t1, 1
add $t2, $t0, $t1
sw $t2, i
