.data
.text
	j main
fact_st_1_0:
	sw $a0,48($sp)
	li $t0,0
	move $t5,$t0
	lw $t4,48($sp)
	move $t2,$t4
	li $t0,1
	move $t1,$t0
	li $t0,1
	move $t3,$t0
	bne $t2,$t1,cond_0_stz_stf_fact_2_0
	j cond_1_after_stf_fact_2_0
cond_0_stz_stf_fact_2_0:
	li $t0,0
	move $t3,$t0
cond_1_after_stf_fact_2_0:
	beq $t3,$t5,if_after2__stf_fact_2_0
	li $t0,1
	move $t1,$t0
	move $v1,$t1
	jr $ra
if_after2__stf_fact_2_0:
	lw $t4,48($sp)
	move $t2,$t4
	li $t0,1
	move $t1,$t0
	sub $t0,$t2,$t1
	move $a0,$t0
	sw $t2,36($sp)
	sw $t1,40($sp)
	sw $t0,44($sp)
	sw $t4,48($sp)
	addiu $sp,$sp,-60
	lw $s0,112($sp)
	sw $s0,52($sp)
	sw $ra,56($sp)
	jal fact_st_1_0
	lw $ra,56($sp)
	lw $s1,52($sp)
	sw $s1,112($sp)
	addiu $sp,$sp,60
	lw $t4,48($sp)
	move $t1,$v1
	lw $t0,52($sp)
	move $t0,$t1
	move $t2,$t4
	move $t1,$t0
	mult $t2,$t1
	mflo $t0
	move $v1,$t0
	jr $ra
main:
	addiu $sp,$sp,-20
	lw $t0,12($sp)
	li $t1,1
	move $t0,$t1
	li $t0,5
	move $t2,$t0
	move $a0,$t2
	sw $t2,0($sp)
	sw $t0,12($sp)
	addiu $sp,$sp,-60
	lw $s0,72($sp)
	sw $s0,52($sp)
	sw $ra,56($sp)
	jal fact_st_1_0
	lw $ra,56($sp)
	lw $s1,52($sp)
	sw $s1,112($sp)
	addiu $sp,$sp,60
	move $t1,$v1
	lw $t0,12($sp)
	move $t0,$t1
	move $t1,$t0
	li $v0,1
	move $a0,$t1
	syscall
	jr $ra
	addiu $sp,$sp,20
