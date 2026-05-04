.text
.globl main
main:
li $t0, 15
li $t1, 5
sll $t2, $t1, 2
sw $t0, w($t2)
li $t3, 10
li $t4, 0
sll $t5, $t4, 2
sw $t3, a($t5)
li $t7, 5
sll $t8, $t7, 2
lw $t6, w($t8)
li $t10, 0
sll $t11, $t10, 2
lw $t9, a($t11)
add $t12, $t6, $t9
sw $t12, i
