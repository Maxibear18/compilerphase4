.text
.globl main
main:
li $t0, 15
li $t1, 5
sll $t2, $t1, 2
sw $t0, w($t2)
li $t0, 10
li $t1, 0
sll $t2, $t1, 2
sw $t0, a($t2)
li $t0, 5
sll $t1, $t0, 2
lw $t2, w($t1)
li $t3, 0
sll $t4, $t3, 2
lw $t5, a($t4)
add $t6, $t2, $t5
sw $t6, i
