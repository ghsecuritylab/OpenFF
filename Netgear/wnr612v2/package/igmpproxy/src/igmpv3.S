	.file	1 "igmpv3.c"
	.section .mdebug.abi32
	.previous
	.abicalls
	.section	.data.rel,"aw",@progbits
	.align	2
	.type	host_op, @object
	.size	host_op, 252
host_op:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.space	12
	.word	0
	.word	0
	.word	0
	.word	set_assign
	.word	0
	.word	0
	.word	set_assign
	.word	0
	.word	1
	.word	set_assign
	.word	0
	.word	0
	.word	set_assign
	.word	0
	.word	1
	.word	set_add
	.word	0
	.word	0
	.word	set_subtract
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	set_assign
	.word	0
	.word	1
	.word	set_assign
	.word	0
	.word	0
	.word	set_assign
	.word	0
	.word	1
	.word	set_assign
	.word	0
	.word	0
	.word	set_subtract
	.word	0
	.word	0
	.word	set_add
	.word	0
	.word	0
	.align	2
	.type	proxy_op, @object
	.size	proxy_op, 252
proxy_op:
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.space	12
	.word	0
	.word	0
	.word	0
	.word	set_add
	.word	0
	.word	0
	.word	set_subtract
	.word	1
	.word	1
	.word	set_add
	.word	0
	.word	0
	.word	set_subtract
	.word	1
	.word	1
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	set_subtract
	.word	0
	.word	0
	.word	set_intersection
	.word	0
	.word	0
	.word	set_subtract
	.word	0
	.word	0
	.word	set_intersection
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.section	.rodata.str1.4,"aMS",@progbits,1
	.subsection	-1
	.align	2
$LC1:
	.ascii	"\000"
	.align	2
$LC2:
	.ascii	"!\000"
	.align	2
$LC0:
	.ascii	"ebtables -D port%d -p ipv4 --ip-dst %u.%u.%u.%u \000"
	.align	2
$LC3:
	.ascii	"%s --ip-msip \000"
	.align	2
$LC5:
	.ascii	",\000"
	.align	2
$LC4:
	.ascii	"%u.%u.%u.%u%s\000"
	.align	2
$LC6:
	.ascii	" -j ACCEPT\000"
	.align	2
$LC7:
	.ascii	"echo \"d %u.%u.%u.%u %d %d %d \000"
	.align	2
$LC8:
	.ascii	"%u.%u.%u.%u \000"
	.align	2
$LC9:
	.ascii	" \" > /proc/mcast_entry\000"
	.align	2
$LC10:
	.ascii	"ebtables -I port%d -p ipv4 --ip-dst %u.%u.%u.%u \000"
	.align	2
$LC11:
	.ascii	"echo \"a %u.%u.%u.%u %d %d %d \000"
	.text
	.align	2
	.ent	update_port_source_info
	.type	update_port_source_info, @function
update_port_source_info:
	.frame	$sp,832,$31		# vars= 744, regs= 10/0, args= 40, gp= 8
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-832
	sw	$31,828($sp)
	sw	$fp,824($sp)
	sw	$23,820($sp)
	sw	$22,816($sp)
	sw	$21,812($sp)
	sw	$20,808($sp)
	sw	$19,804($sp)
	sw	$18,800($sp)
	sw	$17,796($sp)
	sw	$16,792($sp)
	.cprestore	40
	lw	$25,%call16(memset)($28)
	sw	$6,840($sp)
	lw	$16,208($4)
	move	$23,$4
	move	$21,$5
	addiu	$4,$sp,48
	move	$5,$0
	jalr	$25
	li	$6,36			# 0x24

	li	$2,255			# 0xff
	sw	$2,48($sp)
	li	$2,1			# 0x1
	lw	$28,40($sp)
	beq	$16,$0,$L2
	sw	$2,764($sp)

$L14:
	lw	$2,52($16)
	lw	$18,56($16)
	bne	$2,$21,$L5
	li	$2,255			# 0xff

	lw	$3,48($sp)
	addiu	$4,$16,8
	bne	$3,$2,$L6
	addiu	$8,$16,40

	lw	$16,4($16)
	move	$6,$4
	sw	$16,764($sp)
	addiu	$7,$sp,48
$L7:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L7
	addiu	$7,$7,16

	lw	$2,0($8)
	b	$L5
	sw	$2,0($7)

$L6:
	addiu	$9,$sp,84
	move	$6,$4
	move	$7,$9
$L9:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L9
	addiu	$7,$7,16

	lw	$3,0($6)
	lw	$4,764($sp)
	sw	$3,0($7)
	lw	$5,764($sp)
	sll	$2,$4,2
	lw	$4,4($16)
	addu	$2,$2,$5
	sll	$3,$4,1
	sll	$2,$2,2
	addu	$2,$2,$5
	addu	$3,$3,$4
	addu	$3,$3,$2
	lw	$2,%got(proxy_op)($28)
	sll	$3,$3,2
	addiu	$2,$2,%lo(proxy_op)
	addu	$17,$3,$2
	lw	$7,4($17)
	addiu	$16,$sp,120
	lw	$25,0($17)
	move	$6,$16
	move	$5,$9
	jalr	$25
	addiu	$4,$sp,48

	lw	$28,40($sp)
	addiu	$6,$sp,48
	addiu	$7,$sp,152
$L10:
	lw	$2,0($16)
	lw	$3,4($16)
	lw	$4,8($16)
	lw	$5,12($16)
	addiu	$16,$16,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$16,$7,$L10
	addiu	$6,$6,16

	lw	$2,0($16)
	lw	$3,8($17)
	nop
	beq	$3,$0,$L5
	sw	$2,0($6)

	lw	$3,764($sp)
	nop
	xori	$2,$3,0x1
	sltu	$2,$0,$2
	li	$3,2			# 0x2
	subu	$3,$3,$2
	sw	$3,764($sp)
$L5:
	bne	$18,$0,$L14
	move	$16,$18

$L2:
	sll	$4,$21,2
	addu	$16,$4,$23
	lw	$2,76($16)
	lw	$5,764($sp)
	nop
	bne	$2,$5,$L16
	sw	$4,768($sp)

	sll	$4,$21,3
	addu	$4,$4,$21
	sll	$4,$4,2
	lw	$25,%call16(set_comp)($28)
	addu	$4,$23,$4
	addiu	$4,$4,88
	jalr	$25
	addiu	$5,$sp,48

	lw	$28,40($sp)
	beq	$2,$0,$L1
	nop

$L16:
	sll	$2,$21,3
	sw	$2,788($sp)
	lw	$2,48($sp)
	li	$4,255			# 0xff
	lw	$20,76($16)
	lw	$fp,8($23)
	bne	$2,$4,$L17
	nop

	lw	$3,788($sp)
	nop
	addu	$2,$3,$21
	sll	$2,$2,2
	addu	$2,$23,$2
	lw	$3,88($2)
	addiu	$2,$2,88
	sw	$fp,752($sp)
	sw	$2,772($sp)
	bne	$3,$4,$L115
	move	$19,$3

	move	$19,$0
$L115:
	lbu	$2,753($sp)
	lbu	$3,754($sp)
	lbu	$4,755($sp)
	sw	$2,16($sp)
	li	$2,1			# 0x1
	sw	$3,20($sp)
	sw	$4,24($sp)
	addiu	$16,$sp,160
	lbu	$7,752($sp)
	bne	$20,$2,$L21
	addiu	$22,$sp,752

	lw	$2,%got($LC1)($28)
	b	$L22
	addiu	$2,$2,%lo($LC1)

$L21:
	lw	$2,%got($LC2)($28)
	nop
	addiu	$2,$2,%lo($LC2)
$L22:
	lw	$5,%got($LC0)($28)
	lw	$25,%call16(sprintf)($28)
	sw	$2,28($sp)
	addiu	$5,$5,%lo($LC0)
	move	$4,$16
	jalr	$25
	move	$6,$21

	lw	$28,40($sp)
	beq	$19,$0,$L23
	move	$17,$2

	addu	$4,$16,$2
	li	$2,1			# 0x1
	bne	$20,$2,$L26
	nop

	lw	$2,%got($LC1)($28)
	b	$L27
	addiu	$6,$2,%lo($LC1)

$L26:
	lw	$2,%got($LC2)($28)
	nop
	addiu	$6,$2,%lo($LC2)
$L27:
	lw	$5,%got($LC3)($28)
	lw	$25,%call16(sprintf)($28)
	nop
	jalr	$25
	addiu	$5,$5,%lo($LC3)

	lw	$28,40($sp)
	addu	$17,$17,$2
$L23:
	slt	$4,$0,$19
	sw	$4,780($sp)
	beq	$4,$0,$L102
	move	$18,$0

	lw	$16,772($sp)
	nop
$L33:
	lbu	$2,6($16)
	lbu	$6,4($16)
	lbu	$7,5($16)
	sw	$2,16($sp)
	lw	$2,%got($LC5)($28)
	lbu	$3,7($16)
	addiu	$5,$2,%lo($LC5)
	addiu	$4,$19,-1
	addiu	$2,$sp,160
	sw	$3,20($sp)
	bne	$18,$4,$L32
	addu	$3,$2,$17

	lw	$2,%got($LC1)($28)
	nop
	addiu	$5,$2,%lo($LC1)
$L32:
	sw	$5,24($sp)
	lw	$5,%got($LC4)($28)
	lw	$25,%call16(sprintf)($28)
	move	$4,$3
	jalr	$25
	addiu	$5,$5,%lo($LC4)

	addiu	$18,$18,1
	addu	$17,$17,$2
	slt	$2,$18,$19
	lw	$28,40($sp)
	bne	$2,$0,$L33
	addiu	$16,$16,4

$L102:
	lw	$3,%got($LC6)($28)
	addiu	$fp,$sp,48
	lw	$4,%lo($LC6)($3)
	addu	$2,$fp,$17
	addiu	$3,$3,%lo($LC6)
	swl	$4,112($2)
	lw	$5,4($3)
	swr	$4,115($2)
	lbu	$6,8($3)
	lbu	$4,10($3)
	lbu	$3,9($3)
	swl	$5,116($2)
	lw	$25,%call16(system)($28)
	swr	$5,119($2)
	sb	$4,122($2)
	sb	$6,120($2)
	sb	$3,121($2)
	jalr	$25
	addiu	$4,$sp,160

	lw	$28,40($sp)
	lbu	$4,3($22)
	lbu	$3,2($22)
	lw	$5,%got($LC7)($28)
	xori	$2,$20,0x1
	lbu	$7,1($22)
	sltu	$2,$2,1
	lw	$25,%call16(sprintf)($28)
	lbu	$6,752($sp)
	addiu	$20,$sp,416
	sw	$4,20($sp)
	addiu	$5,$5,%lo($LC7)
	sw	$3,16($sp)
	sw	$2,32($sp)
	sw	$21,24($sp)
	sw	$19,28($sp)
	jalr	$25
	move	$4,$20

	lw	$5,780($sp)
	lw	$28,40($sp)
	beq	$5,$0,$L104
	move	$17,$2

	lw	$16,772($sp)
	move	$18,$19
$L38:
	lbu	$2,6($16)
	lbu	$6,4($16)
	lbu	$7,5($16)
	sw	$2,16($sp)
	lbu	$2,7($16)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$17
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$18,$18,-1
	lw	$28,40($sp)
	addu	$17,$17,$2
	bne	$18,$0,$L38
	addiu	$16,$16,4

$L104:
	lw	$3,%got($LC9)($28)
	addu	$2,$fp,$17
	lw	$4,%lo($LC9)($3)
	addiu	$3,$3,%lo($LC9)
	swl	$4,368($2)
	lw	$5,4($3)
	swr	$4,371($2)
	swl	$5,372($2)
	lw	$4,8($3)
	swr	$5,375($2)
	swl	$4,376($2)
	lw	$5,12($3)
	swr	$4,379($2)
	swl	$5,380($2)
	lw	$4,16($3)
	swr	$5,383($2)
	lbu	$5,20($3)
	swl	$4,384($2)
	lbu	$6,21($3)
	swr	$4,387($2)
	sb	$5,388($2)
	lbu	$3,22($3)
	lw	$25,%call16(system)($28)
	sb	$6,389($2)
	sb	$3,390($2)
	jalr	$25
	addiu	$4,$sp,416

	lw	$3,788($sp)
	lw	$28,40($sp)
	addu	$2,$3,$21
	sll	$2,$2,2
	addu	$2,$2,$23
	move	$7,$fp
	addiu	$6,$2,88
	addiu	$8,$sp,80
$L40:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L40
	addiu	$6,$6,16

	lw	$2,0($7)
	lw	$4,768($sp)
	sw	$2,0($6)
	addu	$3,$4,$23
	li	$2,1			# 0x1
	b	$L41
	sw	$2,76($3)

$L17:
	lw	$5,788($sp)
	nop
	addu	$2,$5,$21
	sll	$2,$2,2
	addu	$2,$23,$2
	lw	$3,88($2)
	addiu	$2,$2,88
	sw	$fp,756($sp)
	sw	$2,776($sp)
	bne	$3,$4,$L116
	move	$19,$3

	move	$19,$0
$L116:
	lbu	$2,757($sp)
	lbu	$3,758($sp)
	lbu	$4,759($sp)
	sw	$2,16($sp)
	li	$2,1			# 0x1
	sw	$3,20($sp)
	sw	$4,24($sp)
	addiu	$16,$sp,416
	lbu	$7,756($sp)
	bne	$20,$2,$L45
	addiu	$22,$sp,756

	lw	$2,%got($LC1)($28)
	b	$L46
	addiu	$2,$2,%lo($LC1)

$L45:
	lw	$2,%got($LC2)($28)
	nop
	addiu	$2,$2,%lo($LC2)
$L46:
	lw	$5,%got($LC0)($28)
	lw	$25,%call16(sprintf)($28)
	sw	$2,28($sp)
	addiu	$5,$5,%lo($LC0)
	move	$4,$16
	jalr	$25
	move	$6,$21

	lw	$28,40($sp)
	beq	$19,$0,$L47
	move	$17,$2

	addu	$4,$16,$2
	li	$2,1			# 0x1
	bne	$20,$2,$L50
	nop

	lw	$2,%got($LC1)($28)
	b	$L51
	addiu	$6,$2,%lo($LC1)

$L50:
	lw	$2,%got($LC2)($28)
	nop
	addiu	$6,$2,%lo($LC2)
$L51:
	lw	$5,%got($LC3)($28)
	lw	$25,%call16(sprintf)($28)
	nop
	jalr	$25
	addiu	$5,$5,%lo($LC3)

	lw	$28,40($sp)
	addu	$17,$17,$2
$L47:
	slt	$2,$0,$19
	sw	$2,784($sp)
	beq	$2,$0,$L106
	move	$18,$0

	lw	$16,776($sp)
	nop
$L57:
	lbu	$2,6($16)
	lbu	$6,4($16)
	lbu	$7,5($16)
	sw	$2,16($sp)
	lw	$2,%got($LC5)($28)
	lbu	$3,7($16)
	addiu	$5,$2,%lo($LC5)
	addiu	$4,$19,-1
	addiu	$2,$sp,416
	sw	$3,20($sp)
	bne	$18,$4,$L56
	addu	$3,$2,$17

	lw	$2,%got($LC1)($28)
	nop
	addiu	$5,$2,%lo($LC1)
$L56:
	sw	$5,24($sp)
	lw	$5,%got($LC4)($28)
	lw	$25,%call16(sprintf)($28)
	move	$4,$3
	jalr	$25
	addiu	$5,$5,%lo($LC4)

	addiu	$18,$18,1
	addu	$17,$17,$2
	slt	$2,$18,$19
	lw	$28,40($sp)
	bne	$2,$0,$L57
	addiu	$16,$16,4

$L106:
	lw	$5,%got($LC6)($28)
	addiu	$3,$sp,48
	lw	$4,%lo($LC6)($5)
	addu	$2,$3,$17
	addiu	$3,$5,%lo($LC6)
	swl	$4,368($2)
	lw	$5,4($3)
	swr	$4,371($2)
	lbu	$6,8($3)
	lbu	$4,10($3)
	lbu	$3,9($3)
	swl	$5,372($2)
	lw	$25,%call16(system)($28)
	swr	$5,375($2)
	sb	$4,378($2)
	sb	$6,376($2)
	sb	$3,377($2)
	jalr	$25
	addiu	$4,$sp,416

	lw	$28,40($sp)
	lbu	$4,3($22)
	lbu	$3,2($22)
	lw	$5,%got($LC7)($28)
	xori	$2,$20,0x1
	lbu	$7,1($22)
	sltu	$2,$2,1
	lw	$25,%call16(sprintf)($28)
	lbu	$6,756($sp)
	addiu	$20,$sp,160
	sw	$4,20($sp)
	sw	$3,16($sp)
	sw	$2,32($sp)
	sw	$21,24($sp)
	sw	$19,28($sp)
	addiu	$5,$5,%lo($LC7)
	jalr	$25
	move	$4,$20

	move	$17,$2
	lw	$2,784($sp)
	lw	$28,40($sp)
	beq	$2,$0,$L108
	nop

	lw	$16,776($sp)
	move	$18,$19
$L62:
	lbu	$2,6($16)
	lbu	$6,4($16)
	lbu	$7,5($16)
	sw	$2,16($sp)
	lbu	$2,7($16)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$17
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$18,$18,-1
	lw	$28,40($sp)
	addu	$17,$17,$2
	bne	$18,$0,$L62
	addiu	$16,$16,4

$L108:
	lw	$5,%got($LC9)($28)
	addiu	$3,$sp,48
	lw	$4,%lo($LC9)($5)
	addu	$2,$3,$17
	addiu	$3,$5,%lo($LC9)
	swl	$4,112($2)
	lw	$5,4($3)
	swr	$4,115($2)
	swl	$5,116($2)
	lw	$4,8($3)
	swr	$5,119($2)
	swl	$4,120($2)
	lw	$5,12($3)
	swr	$4,123($2)
	swl	$5,124($2)
	lw	$4,16($3)
	swr	$5,127($2)
	lbu	$5,20($3)
	swl	$4,128($2)
	lbu	$6,21($3)
	swr	$4,131($2)
	sb	$5,132($2)
	lbu	$3,22($3)
	lw	$25,%call16(system)($28)
	sb	$3,134($2)
	sb	$6,133($2)
	jalr	$25
	addiu	$4,$sp,160

	lw	$3,48($sp)
	li	$2,255			# 0xff
	lw	$28,40($sp)
	sw	$fp,760($sp)
	bne	$3,$2,$L117
	move	$19,$3

	move	$19,$0
$L117:
	lbu	$4,763($sp)
	lw	$5,%got($LC10)($28)
	lbu	$2,761($sp)
	lbu	$3,762($sp)
	lw	$25,%call16(sprintf)($28)
	lbu	$7,760($sp)
	sw	$4,24($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	addiu	$5,$5,%lo($LC10)
	addiu	$4,$sp,416
	jalr	$25
	move	$6,$21

	move	$17,$2
	lw	$28,40($sp)
	beq	$19,$0,$L67
	addiu	$20,$sp,760

	addiu	$2,$sp,416
	lw	$3,764($sp)
	addu	$4,$2,$17
	li	$2,1			# 0x1
	bne	$3,$2,$L70
	nop

	lw	$2,%got($LC1)($28)
	b	$L71
	addiu	$6,$2,%lo($LC1)

$L70:
	lw	$2,%got($LC2)($28)
	nop
	addiu	$6,$2,%lo($LC2)
$L71:
	lw	$5,%got($LC3)($28)
	lw	$25,%call16(sprintf)($28)
	nop
	jalr	$25
	addiu	$5,$5,%lo($LC3)

	lw	$28,40($sp)
	addu	$17,$17,$2
$L67:
	slt	$22,$0,$19
	beq	$22,$0,$L110
	move	$18,$0

	addiu	$16,$sp,48
$L77:
	lbu	$3,6($16)
	lbu	$4,7($16)
	lw	$2,%got($LC1)($28)
	sw	$3,16($sp)
	sw	$4,20($sp)
	addiu	$5,$2,%lo($LC1)
	addiu	$3,$sp,416
	addiu	$2,$19,-1
	lbu	$6,4($16)
	lbu	$7,5($16)
	beq	$18,$2,$L76
	addu	$4,$3,$17

	lw	$2,%got($LC5)($28)
	nop
	addiu	$5,$2,%lo($LC5)
$L76:
	sw	$5,24($sp)
	lw	$5,%got($LC4)($28)
	lw	$25,%call16(sprintf)($28)
	addiu	$5,$5,%lo($LC4)
	jalr	$25
	addiu	$18,$18,1

	slt	$3,$18,$19
	addiu	$16,$16,4
	lw	$28,40($sp)
	bne	$3,$0,$L77
	addu	$17,$17,$2

$L110:
	lw	$5,%got($LC6)($28)
	addiu	$3,$sp,48
	lw	$4,%lo($LC6)($5)
	addu	$2,$3,$17
	addiu	$3,$5,%lo($LC6)
	swl	$4,368($2)
	lw	$5,4($3)
	swr	$4,371($2)
	lbu	$6,8($3)
	lbu	$4,10($3)
	lbu	$3,9($3)
	swl	$5,372($2)
	lw	$25,%call16(system)($28)
	swr	$5,375($2)
	sb	$4,378($2)
	sb	$6,376($2)
	sb	$3,377($2)
	jalr	$25
	addiu	$4,$sp,416

	lw	$28,40($sp)
	lw	$5,764($sp)
	lbu	$4,3($20)
	lbu	$3,2($20)
	xori	$2,$5,0x1
	lw	$5,%got($LC11)($28)
	lbu	$7,1($20)
	sltu	$2,$2,1
	lw	$25,%call16(sprintf)($28)
	lbu	$6,760($sp)
	sw	$4,20($sp)
	sw	$3,16($sp)
	sw	$2,32($sp)
	sw	$21,24($sp)
	sw	$19,28($sp)
	addiu	$5,$5,%lo($LC11)
	jalr	$25
	addiu	$4,$sp,160

	lw	$28,40($sp)
	beq	$22,$0,$L112
	move	$17,$2

	move	$18,$19
	addiu	$16,$sp,48
$L82:
	lbu	$2,6($16)
	lbu	$3,7($16)
	lw	$5,%got($LC8)($28)
	lbu	$6,4($16)
	lbu	$7,5($16)
	lw	$25,%call16(sprintf)($28)
	sw	$2,16($sp)
	addiu	$2,$sp,160
	addu	$4,$2,$17
	sw	$3,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$18,$18,-1
	lw	$28,40($sp)
	addu	$17,$17,$2
	bne	$18,$0,$L82
	addiu	$16,$16,4

$L112:
	lw	$3,%got($LC9)($28)
	addiu	$4,$sp,48
	lw	$5,%lo($LC9)($3)
	addu	$2,$4,$17
	addiu	$3,$3,%lo($LC9)
	swl	$5,112($2)
	lw	$4,4($3)
	swr	$5,115($2)
	swl	$4,116($2)
	lw	$5,8($3)
	swr	$4,119($2)
	swl	$5,120($2)
	lw	$4,12($3)
	swr	$5,123($2)
	swl	$4,124($2)
	lw	$5,16($3)
	swr	$4,127($2)
	lbu	$4,20($3)
	swl	$5,128($2)
	lbu	$6,21($3)
	swr	$5,131($2)
	sb	$4,132($2)
	lbu	$3,22($3)
	lw	$25,%call16(system)($28)
	sb	$6,133($2)
	sb	$3,134($2)
	jalr	$25
	addiu	$4,$sp,160

	lw	$5,788($sp)
	lw	$28,40($sp)
	addu	$2,$5,$21
	sll	$2,$2,2
	addu	$2,$2,$23
	addiu	$7,$sp,48
	addiu	$6,$2,88
	addiu	$8,$sp,80
$L85:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L85
	addiu	$6,$6,16

	lw	$2,0($7)
	lw	$4,768($sp)
	sw	$2,0($6)
	lw	$5,764($sp)
	addu	$3,$4,$23
	sw	$5,76($3)
$L41:
	lw	$2,840($sp)
	nop
	beq	$2,$0,$L1
	addiu	$6,$23,88

	lw	$17,76($23)
	addiu	$7,$sp,672
	addiu	$8,$23,120
$L87:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L87
	addiu	$7,$7,16

	lw	$2,0($6)
	lw	$25,%call16(memset)($28)
	sw	$2,0($7)
	addiu	$4,$sp,712
	move	$5,$0
	jalr	$25
	li	$6,36			# 0x24

	lw	$28,40($sp)
	li	$22,1			# 0x1
	addiu	$19,$23,124
	addiu	$18,$23,80
$L95:
	lw	$3,0($18)
	sll	$2,$17,2
	addu	$2,$2,$17
	sll	$16,$3,1
	sll	$2,$2,2
	addu	$16,$16,$3
	addu	$2,$2,$17
	addu	$16,$16,$2
	lw	$2,%got(proxy_op)($28)
	sll	$16,$16,2
	addiu	$2,$2,%lo(proxy_op)
	addu	$16,$16,$2
	addiu	$21,$sp,672
	addiu	$20,$sp,712
	lw	$25,0($16)
	lw	$7,4($16)
	move	$4,$21
	move	$5,$19
	jalr	$25
	move	$6,$20

	lw	$2,8($16)
	lw	$28,40($sp)
	beq	$2,$0,$L118
	move	$6,$20

	xori	$2,$17,0x1
	sltu	$2,$0,$2
	li	$3,2			# 0x2
	subu	$17,$3,$2
$L118:
	move	$7,$21
	addiu	$8,$sp,744
$L94:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L94
	addiu	$7,$7,16

	lw	$2,0($6)
	addiu	$22,$22,1
	slt	$3,$22,3
	sw	$2,0($7)
	addiu	$18,$18,4
	bne	$3,$0,$L95
	addiu	$19,$19,36

	lw	$2,36($23)
	nop
	bne	$17,$2,$L97
	move	$5,$21

	lw	$25,%call16(set_comp)($28)
	nop
	jalr	$25
	addiu	$4,$23,40

	lw	$28,40($sp)
	beq	$2,$0,$L1
	nop

$L97:
	sw	$17,36($23)
	addiu	$7,$sp,672
	addiu	$6,$23,40
	addiu	$8,$sp,704
$L98:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L98
	addiu	$6,$6,16

	lw	$2,0($7)
	lw	$25,%call16(set_source_list)($28)
	sw	$2,0($6)
	jalr	$25
	move	$4,$23

	lw	$28,40($sp)
$L1:
	lw	$31,828($sp)
	lw	$fp,824($sp)
	lw	$23,820($sp)
	lw	$22,816($sp)
	lw	$21,812($sp)
	lw	$20,808($sp)
	lw	$19,804($sp)
	lw	$18,800($sp)
	lw	$17,796($sp)
	lw	$16,792($sp)
	j	$31
	addiu	$sp,$sp,832

	.set	macro
	.set	reorder
	.end	update_port_source_info
	.align	2
	.globl	update_all_ports
	.ent	update_all_ports
	.type	update_all_ports, @function
update_all_ports:
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$31,32($sp)
	sw	$17,28($sp)
	sw	$16,24($sp)
	.cprestore	16
	lw	$25,%call16(mc_ctrl_packet)($28)
	move	$17,$4
	lw	$4,8($4)
	jalr	$25
	nop

	lw	$28,16($sp)
	bne	$2,$0,$L119
	move	$16,$0

$L124:
	lw	$25,%got(update_port_source_info)($28)
	xori	$6,$16,0x2
	move	$5,$16
	sltu	$6,$6,1
	addiu	$16,$16,1
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	move	$4,$17

	slt	$2,$16,3
	lw	$28,16($sp)
	bne	$2,$0,$L124
	nop

$L119:
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	update_all_ports
	.section	.rodata.str1.4
	.align	2
$LC12:
	.ascii	"echo \"%u.%u.%u.%u %u.%u.%u.%u %d %d \000"
	.align	2
$LC13:
	.ascii	"\" > /proc/mcast_set\000"
	.align	2
$LC14:
	.ascii	"\" > /proc/wl_mcast_set\000"
	.text
	.align	2
	.globl	processV3report
	.ent	processV3report
	.type	processV3report, @function
processV3report:
	.frame	$sp,2824,$31		# vars= 2728, regs= 10/0, args= 48, gp= 8
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-2824
	sw	$31,2820($sp)
	sw	$fp,2816($sp)
	sw	$23,2812($sp)
	sw	$22,2808($sp)
	sw	$21,2804($sp)
	sw	$20,2800($sp)
	sw	$19,2796($sp)
	sw	$18,2792($sp)
	sw	$17,2788($sp)
	sw	$16,2784($sp)
	.cprestore	48
	lhu	$2,6($4)
	sw	$5,2828($sp)
	sw	$2,2772($sp)
	move	$21,$6
	addiu	$23,$4,8
	sw	$0,2776($sp)
	beq	$2,$0,$L126
	sw	$0,2768($sp)

$L368:
	beq	$23,$0,$L126
	li	$3,-536870912			# 0xffffffffe0000000

	lw	$20,4($23)
	li	$2,-268435456			# 0xfffffffff0000000
	and	$2,$20,$2
	bne	$2,$3,$L126
	move	$4,$0

	lhu	$3,2($23)
	nop
	sltu	$2,$3,9
	beq	$2,$0,$L126
	nop

	lbu	$2,0($23)
	nop
	sltu	$2,$2,7
	beq	$2,$0,$L126
	nop

	beq	$3,$0,$L331
	move	$5,$0

	move	$6,$3
	li	$8,-268435456			# 0xfffffffff0000000
	li	$7,-536870912			# 0xffffffffe0000000
	addiu	$3,$23,8
$L140:
	lw	$2,0($3)
	nop
	bne	$2,$0,$L137
	nop

	addiu	$4,$4,1
$L137:
	and	$2,$2,$8
	bne	$2,$7,$L377
	slt	$2,$4,2

	addiu	$4,$4,2
	slt	$2,$4,2
$L377:
	beq	$2,$0,$L126
	addiu	$5,$5,1

	slt	$2,$5,$6
	bne	$2,$0,$L140
	addiu	$3,$3,4

$L331:
	li	$2,-268435456			# 0xfffffffff0000000
	and	$2,$20,$2
	li	$3,-536870912			# 0xffffffffe0000000
	bne	$2,$3,$L129
	nop

	lw	$25,%call16(mc_ctrl_packet)($28)
	nop
	jalr	$25
	move	$4,$20

	lw	$28,48($sp)
	bne	$2,$0,$L129
	nop

	lw	$25,%call16(findRoute)($28)
	nop
	jalr	$25
	move	$4,$20

	lw	$28,48($sp)
	addiu	$fp,$sp,56
	lw	$25,%call16(memset)($28)
	move	$5,$0
	move	$4,$fp
	li	$6,36			# 0x24
	jalr	$25
	move	$18,$2

	lhu	$2,2($23)
	lw	$28,48($sp)
	move	$5,$0
	beq	$2,$0,$L333
	sw	$2,56($sp)

	move	$6,$2
$L146:
	sll	$3,$5,2
	addu	$2,$3,$23
	lw	$4,8($2)
	addiu	$5,$5,1
	addu	$3,$3,$fp
	slt	$2,$5,$6
	bne	$2,$0,$L146
	sw	$4,4($3)

$L333:
	bne	$18,$0,$L148
	nop

	lbu	$16,0($23)
	nop
	sltu	$2,$16,6
	beq	$2,$0,$L129
	li	$2,1			# 0x1

	beq	$16,$2,$L151
	li	$2,3			# 0x3

	bne	$16,$2,$L150
	nop

$L151:
	lw	$2,56($sp)
	nop
	beq	$2,$0,$L129
	nop

$L150:
	lw	$25,%call16(acceptGroupReport)($28)
	lw	$5,2828($sp)
	move	$4,$21
	move	$6,$20
	jalr	$25
	li	$7,34			# 0x22

	lw	$28,48($sp)
	nop
	lw	$25,%call16(findRoute)($28)
	nop
	jalr	$25
	move	$4,$20

	lw	$28,48($sp)
	beq	$2,$0,$L129
	move	$17,$2

	move	$6,$fp
	addiu	$7,$2,40
	addiu	$18,$fp,32
$L154:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$18,$L154
	addiu	$7,$7,16

	lw	$2,0($18)
	nop
	sw	$2,0($7)
	li	$2,1			# 0x1
	beq	$16,$2,$L157
	li	$2,3			# 0x3

	beq	$16,$2,$L157
	li	$2,5			# 0x5

	bne	$16,$2,$L156
	li	$19,2			# 0x2

$L157:
	li	$19,1			# 0x1
$L156:
	lw	$25,%call16(findListener)($28)
	lw	$5,2828($sp)
	jalr	$25
	move	$4,$17

	lw	$28,48($sp)
	beq	$2,$0,$L158
	move	$16,$2

	move	$6,$fp
	addiu	$7,$2,8
$L159:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$18,$L159
	addiu	$7,$7,16

	lw	$2,0($6)
	nop
	sw	$2,0($7)
	lw	$3,8($16)
	li	$2,34			# 0x22
	sw	$19,4($16)
	sw	$2,0($16)
	beq	$3,$0,$L158
	sw	$21,52($16)

	lw	$25,%call16(add_source_list)($28)
	lw	$4,8($17)
	lw	$5,12($16)
	jalr	$25
	nop

	lw	$28,48($sp)
	nop
$L158:
	lw	$25,%got(update_port_source_info)($28)
	move	$4,$17
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	lw	$2,8($17)
	lw	$25,%call16(sprintf)($28)
	sw	$2,2736($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2736($sp)
	lbu	$4,2737($sp)
	lbu	$5,2738($sp)
	lbu	$8,2739($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	addiu	$22,$sp,136
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	move	$4,$22
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	addiu	$20,$sp,2736
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L335
	move	$19,$0

	move	$17,$16
$L165:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L165
	addiu	$17,$17,4

$L335:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,80($2)
	lw	$5,4($3)
	swr	$4,83($2)
	swl	$5,84($2)
	lw	$4,8($3)
	swr	$5,87($2)
	swl	$4,88($2)
	lw	$5,12($3)
	swr	$4,91($2)
	swl	$5,92($2)
	lw	$3,16($3)
	swr	$5,95($2)
	lw	$25,%call16(system)($28)
	swl	$3,96($2)
	swr	$3,99($2)
	jalr	$25
	addiu	$4,$sp,136

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2736($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,392
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L337
	move	$19,$0

	move	$17,$16
$L169:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L169
	addiu	$17,$17,4

$L337:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,336($2)
	lw	$5,4($3)
	swr	$4,339($2)
	swl	$5,340($2)
	lw	$4,8($3)
	swr	$5,343($2)
	swl	$4,344($2)
	lw	$5,12($3)
	swr	$4,347($2)
	swl	$5,348($2)
	lw	$4,16($3)
	swr	$5,351($2)
	lbu	$5,20($3)
	swl	$4,352($2)
	lbu	$6,21($3)
	swr	$4,355($2)
	lw	$25,%call16(system)($28)
	sb	$5,356($2)
	lbu	$3,22($3)
	addiu	$4,$sp,392
	sb	$6,357($2)
	b	$L375
	sb	$3,358($2)

$L148:
	lw	$2,36($18)
	nop
	sltu	$2,$2,3
	bne	$2,$0,$L172
	nop

	sw	$0,36($18)
$L172:
	lw	$25,%call16(findListener)($28)
	lw	$5,2828($sp)
	jalr	$25
	move	$4,$18

	move	$16,$2
	lw	$2,36($18)
	lw	$28,48($sp)
	bne	$2,$0,$L173
	nop

	lbu	$16,0($23)
	nop
	andi	$3,$16,0x00ff
	sltu	$2,$3,6
	beq	$2,$0,$L129
	li	$2,1			# 0x1

	sll	$3,$2,$3
	andi	$2,$3,0x2a
	beq	$2,$0,$L369
	andi	$2,$3,0x14

	lw	$25,%call16(memcmp)($28)
	lw	$5,%got(ZEROSET)($28)
	addiu	$4,$sp,56
	jalr	$25
	li	$6,36			# 0x24

	lw	$28,48($sp)
$L369:
	beq	$2,$0,$L129
	li	$2,1			# 0x1

	andi	$16,$16,0x00ff
	beq	$16,$2,$L182
	li	$2,3			# 0x3

	bne	$16,$2,$L181
	nop

$L182:
	lw	$2,56($sp)
	nop
	beq	$2,$0,$L129
	nop

$L181:
	lw	$25,%call16(acceptGroupReport)($28)
	lw	$5,2828($sp)
	move	$4,$21
	move	$6,$20
	jalr	$25
	li	$7,34			# 0x22

	lw	$28,48($sp)
	nop
	lw	$25,%call16(findRoute)($28)
	nop
	jalr	$25
	move	$4,$20

	lw	$28,48($sp)
	beq	$2,$0,$L129
	move	$17,$2

	move	$6,$fp
	addiu	$7,$2,40
	addiu	$18,$fp,32
$L185:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$18,$L185
	addiu	$7,$7,16

	lw	$2,0($18)
	nop
	sw	$2,0($7)
	li	$2,1			# 0x1
	beq	$16,$2,$L188
	li	$2,3			# 0x3

	beq	$16,$2,$L188
	li	$2,5			# 0x5

	bne	$16,$2,$L187
	li	$19,2			# 0x2

$L188:
	li	$19,1			# 0x1
$L187:
	lw	$25,%call16(findListener)($28)
	lw	$5,2828($sp)
	jalr	$25
	move	$4,$17

	lw	$28,48($sp)
	beq	$2,$0,$L189
	move	$16,$2

	move	$6,$fp
	addiu	$7,$2,8
$L190:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$18,$L190
	addiu	$7,$7,16

	lw	$2,0($6)
	nop
	sw	$2,0($7)
	lw	$3,8($16)
	li	$2,34			# 0x22
	sw	$19,4($16)
	sw	$2,0($16)
	beq	$3,$0,$L189
	sw	$21,52($16)

	lw	$25,%call16(add_source_list)($28)
	lw	$4,8($17)
	lw	$5,12($16)
	jalr	$25
	nop

	lw	$28,48($sp)
	nop
$L189:
	lw	$25,%got(update_port_source_info)($28)
	move	$4,$17
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	lw	$2,8($17)
	lw	$25,%call16(sprintf)($28)
	sw	$2,2740($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2740($sp)
	lbu	$4,2741($sp)
	lbu	$5,2742($sp)
	lbu	$8,2743($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	addiu	$22,$sp,648
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	move	$4,$22
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	addiu	$20,$sp,2740
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L339
	move	$19,$0

	move	$17,$16
$L196:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L196
	addiu	$17,$17,4

$L339:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,592($2)
	lw	$5,4($3)
	swr	$4,595($2)
	swl	$5,596($2)
	lw	$4,8($3)
	swr	$5,599($2)
	swl	$4,600($2)
	lw	$5,12($3)
	swr	$4,603($2)
	swl	$5,604($2)
	lw	$3,16($3)
	swr	$5,607($2)
	lw	$25,%call16(system)($28)
	swl	$3,608($2)
	swr	$3,611($2)
	jalr	$25
	addiu	$4,$sp,648

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2740($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,904
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L341
	move	$19,$0

	move	$17,$16
$L200:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L200
	addiu	$17,$17,4

$L341:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,848($2)
	lw	$5,4($3)
	swr	$4,851($2)
	swl	$5,852($2)
	lw	$4,8($3)
	swr	$5,855($2)
	swl	$4,856($2)
	lw	$5,12($3)
	swr	$4,859($2)
	swl	$5,860($2)
	lw	$4,16($3)
	swr	$5,863($2)
	lbu	$5,20($3)
	swl	$4,864($2)
	lbu	$6,21($3)
	swr	$4,867($2)
	lw	$25,%call16(system)($28)
	sb	$5,868($2)
	lbu	$3,22($3)
	addiu	$4,$sp,904
	sb	$6,869($2)
	b	$L375
	sb	$3,870($2)

$L173:
	lw	$25,%call16(getIfByAddress)($28)
	lw	$4,2828($sp)
	jalr	$25
	nop

	lw	$28,48($sp)
	beq	$2,$0,$L126
	move	$4,$2

	bne	$16,$0,$L206
	li	$2,6			# 0x6

	lbu	$3,0($23)
	nop
	beq	$3,$2,$L378
	nop

$L206:
	lw	$2,56($sp)
	nop
	bne	$2,$0,$L208
	li	$2,3			# 0x3

	lbu	$3,0($23)
	nop
	beq	$3,$2,$L205
	nop

$L208:
	lw	$6,40($4)
	lw	$25,%call16(insertRoute)($28)
	lw	$7,2828($sp)
	move	$4,$21
	jalr	$25
	move	$5,$20

	lw	$28,48($sp)
$L205:
	beq	$16,$0,$L209
	nop

	lw	$2,52($16)
	nop
	beq	$2,$21,$L210
	li	$5,1			# 0x1

	sw	$5,2776($sp)
$L210:
	lw	$4,4($16)
	lbu	$5,0($23)
	sll	$2,$4,2
	addu	$2,$2,$4
	sll	$3,$5,1
	sll	$2,$2,2
	addu	$2,$2,$4
	addu	$3,$3,$5
	addu	$3,$3,$2
	lw	$2,%got(host_op)($28)
	sll	$3,$3,2
	addiu	$2,$2,%lo(host_op)
	addu	$3,$3,$2
	lw	$7,4($3)
	lw	$25,0($3)
	addiu	$17,$16,8
	addiu	$19,$sp,96
	move	$4,$17
	addiu	$5,$sp,56
	jalr	$25
	move	$6,$19

	lw	$3,4($16)
	lbu	$4,0($23)
	sll	$2,$3,3
	subu	$2,$2,$3
	addu	$2,$2,$4
	addiu	$3,$2,-8
	sltu	$2,$3,13
	lw	$28,48($sp)
	beq	$2,$0,$L129
	sll	$2,$3,2

	lw	$3,%got($L303)($28)
	nop
	addiu	$3,$3,%lo($L303)
	addu	$2,$2,$3
	lw	$2,0($2)
	nop
	addu	$2,$2,$28
	j	$2
	nop

	.rdata
	.align	2
$L303:
	.gpword	$L214
	.gpword	$L233
	.gpword	$L214
	.gpword	$L233
	.gpword	$L214
	.gpword	$L247
	.gpword	$L129
	.gpword	$L266
	.gpword	$L286
	.gpword	$L266
	.gpword	$L286
	.gpword	$L286
	.gpword	$L286
	.text
$L214:
	lw	$2,2776($sp)
	nop
	bne	$2,$0,$L216
	move	$4,$17

	lw	$25,%call16(memcmp)($28)
	move	$5,$19
	jalr	$25
	li	$6,36			# 0x24

	lw	$28,48($sp)
	beq	$2,$0,$L129
	nop

$L216:
	lw	$2,96($sp)
	nop
	beq	$2,$0,$L267
	addiu	$7,$sp,96

	addiu	$6,$16,8
	addiu	$8,$sp,128
$L218:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L218
	addiu	$6,$6,16

	lw	$2,0($7)
	nop
	sw	$2,0($6)
	lw	$4,2776($sp)
	nop
	beq	$4,$0,$L219
	sw	$21,52($16)

	lw	$25,%call16(update_all_ports)($28)
	nop
	jalr	$25
	move	$4,$18

	b	$L370
	nop

$L219:
	lw	$25,%got(update_port_source_info)($28)
	move	$4,$18
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

$L370:
	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	sw	$20,2744($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2744($sp)
	lbu	$4,2745($sp)
	lbu	$5,2746($sp)
	lbu	$8,2747($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$25,%call16(sprintf)($28)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	addiu	$22,$sp,904
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	move	$4,$22
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	slt	$3,$21,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	addiu	$20,$sp,2744
	beq	$3,$0,$L343
	move	$19,$0

	move	$17,$16
$L225:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L225
	addiu	$17,$17,4

$L343:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,848($2)
	lw	$5,4($3)
	swr	$4,851($2)
	swl	$5,852($2)
	lw	$4,8($3)
	swr	$5,855($2)
	swl	$4,856($2)
	lw	$5,12($3)
	swr	$4,859($2)
	swl	$5,860($2)
	lw	$3,16($3)
	swr	$5,863($2)
	lw	$25,%call16(system)($28)
	swl	$3,864($2)
	swr	$3,867($2)
	jalr	$25
	addiu	$4,$sp,904

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2744($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,648
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L345
	move	$19,$0

	move	$17,$16
$L229:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L229
	addiu	$17,$17,4

$L345:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,592($2)
	lw	$5,4($3)
	swr	$4,595($2)
	swl	$5,596($2)
	lw	$4,8($3)
	swr	$5,599($2)
	swl	$4,600($2)
	lw	$5,12($3)
	swr	$4,603($2)
	swl	$5,604($2)
	lw	$4,16($3)
	swr	$5,607($2)
	lbu	$5,20($3)
	swl	$4,608($2)
	lbu	$6,21($3)
	swr	$4,611($2)
	lw	$25,%call16(system)($28)
	sb	$5,612($2)
	lbu	$3,22($3)
	addiu	$4,$sp,648
	sb	$6,613($2)
	b	$L375
	sb	$3,614($2)

$L233:
	li	$2,2			# 0x2
	sw	$2,4($16)
	move	$6,$19
	move	$7,$17
	addiu	$8,$sp,128
$L234:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L234
	addiu	$7,$7,16

	lw	$2,0($6)
	nop
	sw	$2,0($7)
	lw	$5,2776($sp)
	nop
	beq	$5,$0,$L235
	sw	$21,52($16)

	lw	$25,%call16(update_all_ports)($28)
	nop
	jalr	$25
	move	$4,$18

	b	$L371
	nop

$L235:
	lw	$25,%got(update_port_source_info)($28)
	move	$4,$18
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

$L371:
	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	sw	$20,2748($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2748($sp)
	lbu	$4,2749($sp)
	lbu	$5,2750($sp)
	lbu	$8,2751($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$25,%call16(sprintf)($28)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	addiu	$22,$sp,1160
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	move	$4,$22
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	slt	$3,$21,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	addiu	$20,$sp,2748
	beq	$3,$0,$L347
	move	$19,$0

	move	$17,$16
$L241:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L241
	addiu	$17,$17,4

$L347:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,1104($2)
	lw	$5,4($3)
	swr	$4,1107($2)
	swl	$5,1108($2)
	lw	$4,8($3)
	swr	$5,1111($2)
	swl	$4,1112($2)
	lw	$5,12($3)
	swr	$4,1115($2)
	swl	$5,1116($2)
	lw	$3,16($3)
	swr	$5,1119($2)
	lw	$25,%call16(system)($28)
	swl	$3,1120($2)
	swr	$3,1123($2)
	jalr	$25
	addiu	$4,$sp,1160

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2748($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,1416
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L349
	move	$19,$0

	move	$17,$16
$L245:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L245
	addiu	$17,$17,4

$L349:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,1360($2)
	lw	$5,4($3)
	swr	$4,1363($2)
	swl	$5,1364($2)
	lw	$4,8($3)
	swr	$5,1367($2)
	swl	$4,1368($2)
	lw	$5,12($3)
	swr	$4,1371($2)
	swl	$5,1372($2)
	lw	$4,16($3)
	swr	$5,1375($2)
	lbu	$5,20($3)
	swl	$4,1376($2)
	lbu	$6,21($3)
	swr	$4,1379($2)
	lw	$25,%call16(system)($28)
	sb	$5,1380($2)
	lbu	$3,22($3)
	addiu	$4,$sp,1416
	sb	$6,1381($2)
	b	$L375
	sb	$3,1382($2)

$L247:
	lw	$2,96($sp)
	nop
	beq	$2,$0,$L267
	nop

	lw	$5,2776($sp)
	nop
	bne	$5,$0,$L379
	addiu	$7,$sp,96

	lw	$25,%call16(memcmp)($28)
	move	$4,$17
	move	$5,$19
	jalr	$25
	li	$6,36			# 0x24

	lw	$28,48($sp)
	beq	$2,$0,$L129
	addiu	$7,$sp,96

$L379:
	addiu	$6,$16,8
	addiu	$8,$sp,128
$L251:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L251
	addiu	$6,$6,16

	lw	$2,0($7)
	nop
	sw	$2,0($6)
	lw	$2,2776($sp)
	nop
	beq	$2,$0,$L252
	sw	$21,52($16)

	lw	$25,%call16(update_all_ports)($28)
	nop
	jalr	$25
	move	$4,$18

	b	$L372
	nop

$L252:
	lw	$25,%got(update_port_source_info)($28)
	move	$4,$18
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

$L372:
	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	sw	$20,2752($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2752($sp)
	lbu	$4,2753($sp)
	lbu	$5,2754($sp)
	lbu	$8,2755($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$25,%call16(sprintf)($28)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	addiu	$22,$sp,1672
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	move	$4,$22
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	slt	$3,$21,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	addiu	$20,$sp,2752
	beq	$3,$0,$L351
	move	$19,$0

	move	$17,$16
$L258:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L258
	addiu	$17,$17,4

$L351:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,1616($2)
	lw	$5,4($3)
	swr	$4,1619($2)
	swl	$5,1620($2)
	lw	$4,8($3)
	swr	$5,1623($2)
	swl	$4,1624($2)
	lw	$5,12($3)
	swr	$4,1627($2)
	swl	$5,1628($2)
	lw	$3,16($3)
	swr	$5,1631($2)
	lw	$25,%call16(system)($28)
	swl	$3,1632($2)
	swr	$3,1635($2)
	jalr	$25
	addiu	$4,$sp,1672

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2752($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,1928
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L353
	move	$19,$0

	move	$17,$16
$L262:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L262
	addiu	$17,$17,4

$L353:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,1872($2)
	lw	$5,4($3)
	swr	$4,1875($2)
	swl	$5,1876($2)
	lw	$4,8($3)
	swr	$5,1879($2)
	swl	$4,1880($2)
	lw	$5,12($3)
	swr	$4,1883($2)
	swl	$5,1884($2)
	lw	$4,16($3)
	swr	$5,1887($2)
	lbu	$5,20($3)
	swl	$4,1888($2)
	lbu	$6,21($3)
	swr	$4,1891($2)
	lw	$25,%call16(system)($28)
	sb	$5,1892($2)
	lbu	$3,22($3)
	addiu	$4,$sp,1928
	sb	$6,1893($2)
	b	$L375
	sb	$3,1894($2)

$L266:
	lw	$2,96($sp)
	nop
	beq	$2,$0,$L267
	li	$2,1			# 0x1

	lw	$25,%call16(memcmp)($28)
	sw	$2,4($16)
	sw	$21,52($16)
	move	$4,$17
	move	$5,$19
	jalr	$25
	li	$6,36			# 0x24

	lw	$28,48($sp)
	beq	$2,$0,$L268
	move	$6,$19

	move	$7,$17
	addiu	$8,$sp,128
$L269:
	lw	$2,0($6)
	lw	$3,4($6)
	lw	$4,8($6)
	lw	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L269
	addiu	$7,$7,16

	lw	$2,0($6)
	nop
	sw	$2,0($7)
$L268:
	lw	$5,2776($sp)
	nop
	beq	$5,$0,$L270
	move	$4,$18

	lw	$25,%call16(update_all_ports)($28)
	nop
	jalr	$25
	move	$4,$18

	b	$L373
	nop

$L270:
	lw	$25,%got(update_port_source_info)($28)
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

$L373:
	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	sw	$20,2756($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2756($sp)
	lbu	$4,2757($sp)
	lbu	$5,2758($sp)
	lbu	$8,2759($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$25,%call16(sprintf)($28)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	addiu	$22,$sp,1928
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	move	$4,$22
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	slt	$3,$21,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	addiu	$20,$sp,2756
	beq	$3,$0,$L355
	move	$19,$0

	move	$17,$16
$L276:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L276
	addiu	$17,$17,4

$L355:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,1872($2)
	lw	$5,4($3)
	swr	$4,1875($2)
	swl	$5,1876($2)
	lw	$4,8($3)
	swr	$5,1879($2)
	swl	$4,1880($2)
	lw	$5,12($3)
	swr	$4,1883($2)
	swl	$5,1884($2)
	lw	$3,16($3)
	swr	$5,1887($2)
	lw	$25,%call16(system)($28)
	swl	$3,1888($2)
	swr	$3,1891($2)
	jalr	$25
	addiu	$4,$sp,1928

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2756($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,1672
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L357
	move	$19,$0

	move	$17,$16
$L280:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L280
	addiu	$17,$17,4

$L357:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,1616($2)
	lw	$5,4($3)
	swr	$4,1619($2)
	swl	$5,1620($2)
	lw	$4,8($3)
	swr	$5,1623($2)
	swl	$4,1624($2)
	lw	$5,12($3)
	swr	$4,1627($2)
	swl	$5,1628($2)
	lw	$4,16($3)
	swr	$5,1631($2)
	lbu	$5,20($3)
	swl	$4,1632($2)
	lbu	$6,21($3)
	swr	$4,1635($2)
	lw	$25,%call16(system)($28)
	sb	$5,1636($2)
	lbu	$3,22($3)
	addiu	$4,$sp,1672
	sb	$6,1637($2)
	b	$L375
	sb	$3,1638($2)

$L267:
	lw	$25,%call16(acceptLeaveMessage)($28)
	lw	$4,2828($sp)
	jalr	$25
	move	$5,$20

	b	$L376
	nop

$L286:
	lw	$5,2776($sp)
	nop
	bne	$5,$0,$L288
	move	$4,$17

	lw	$25,%call16(memcmp)($28)
	move	$5,$19
	jalr	$25
	li	$6,36			# 0x24

	lw	$28,48($sp)
	beq	$2,$0,$L129
	nop

$L288:
	sw	$21,52($16)
	addiu	$7,$sp,96
	addiu	$6,$16,8
	addiu	$8,$sp,128
$L289:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L289
	addiu	$6,$6,16

	lw	$2,0($7)
	nop
	sw	$2,0($6)
	lw	$2,2776($sp)
	nop
	beq	$2,$0,$L290
	move	$4,$18

	lw	$25,%call16(update_all_ports)($28)
	nop
	jalr	$25
	move	$4,$18

	b	$L374
	nop

$L290:
	lw	$25,%got(update_port_source_info)($28)
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

$L374:
	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	sw	$20,2760($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2760($sp)
	lbu	$4,2761($sp)
	lbu	$5,2762($sp)
	lbu	$8,2763($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$25,%call16(sprintf)($28)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	addiu	$22,$sp,2184
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	move	$4,$22
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	slt	$3,$21,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	addiu	$20,$sp,2760
	beq	$3,$0,$L359
	move	$19,$0

	move	$17,$16
$L296:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L296
	addiu	$17,$17,4

$L359:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,2128($2)
	lw	$5,4($3)
	swr	$4,2131($2)
	swl	$5,2132($2)
	lw	$4,8($3)
	swr	$5,2135($2)
	swl	$4,2136($2)
	lw	$5,12($3)
	swr	$4,2139($2)
	swl	$5,2140($2)
	lw	$3,16($3)
	swr	$5,2143($2)
	lw	$25,%call16(system)($28)
	swl	$3,2144($2)
	swr	$3,2147($2)
	jalr	$25
	addiu	$4,$sp,2184

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2760($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,2440
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L361
	move	$19,$0

	move	$17,$16
$L300:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L300
	addiu	$17,$17,4

$L361:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,2384($2)
	lw	$5,4($3)
	swr	$4,2387($2)
	swl	$5,2388($2)
	lw	$4,8($3)
	swr	$5,2391($2)
	swl	$4,2392($2)
	lw	$5,12($3)
	swr	$4,2395($2)
	swl	$5,2396($2)
	lw	$4,16($3)
	swr	$5,2399($2)
	lbu	$5,20($3)
	swl	$4,2400($2)
	lbu	$6,21($3)
	swr	$4,2403($2)
	lw	$25,%call16(system)($28)
	sb	$5,2404($2)
	lbu	$3,22($3)
	addiu	$4,$sp,2440
	sb	$6,2405($2)
	b	$L375
	sb	$3,2406($2)

$L209:
	lbu	$3,0($23)
	nop
$L378:
	andi	$4,$3,0x00ff
	sltu	$2,$4,6
	beq	$2,$0,$L129
	li	$2,1			# 0x1

	sll	$4,$2,$4
	andi	$2,$4,0x2a
	bne	$2,$0,$L305
	li	$17,1			# 0x1

	andi	$2,$4,0x14
	beq	$2,$0,$L129
	li	$17,2			# 0x2

$L305:
	andi	$3,$3,0x00ff
	li	$2,3			# 0x3
	bne	$3,$2,$L313
	nop

	lw	$2,56($sp)
	nop
	beq	$2,$0,$L129
	nop

$L313:
	lw	$25,%call16(findListener)($28)
	lw	$5,2828($sp)
	jalr	$25
	move	$4,$18

	lw	$28,48($sp)
	bne	$2,$0,$L315
	move	$16,$2

	lw	$25,%call16(insertListener)($28)
	lw	$6,2828($sp)
	move	$4,$21
	jalr	$25
	move	$5,$18

	lw	$28,48($sp)
	beq	$2,$0,$L129
	move	$16,$2

$L315:
	li	$2,34			# 0x22
	sw	$17,4($16)
	sw	$2,0($16)
	sw	$21,52($16)
	move	$7,$fp
	addiu	$6,$16,8
	addiu	$8,$sp,88
$L317:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L317
	addiu	$6,$6,16

	lw	$25,%got(update_port_source_info)($28)
	lw	$2,0($7)
	move	$4,$18
	sw	$2,0($6)
	move	$5,$21
	addiu	$25,$25,%lo(update_port_source_info)
	jalr	$25
	li	$6,1			# 0x1

	lw	$28,48($sp)
	bne	$21,$0,$L129
	nop

	lw	$25,%call16(join_group)($28)
	lw	$5,8($18)
	lw	$4,2828($sp)
	jalr	$25
	addiu	$22,$sp,2440

	lw	$28,48($sp)
	lw	$5,8($18)
	lw	$25,%call16(wireless_join_group)($28)
	lw	$4,2828($sp)
	jalr	$25
	addiu	$20,$sp,2764

	lw	$2,8($18)
	lw	$28,48($sp)
	sw	$2,2764($sp)
	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$2,47($16)
	lbu	$3,2764($sp)
	lbu	$4,2765($sp)
	lbu	$5,2766($sp)
	lbu	$8,2767($sp)
	sw	$3,24($sp)
	sw	$4,28($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$25,%call16(sprintf)($28)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$2,8($16)
	move	$4,$22
	sw	$2,44($sp)
	lw	$2,%got($LC12)($28)
	move	$19,$0
	jalr	$25
	addiu	$5,$2,%lo($LC12)

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	blez	$3,$L363
	sra	$18,$18,24

	move	$17,$16
$L322:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$22,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L322
	addiu	$17,$17,4

$L363:
	lw	$3,%got($LC13)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC13)($3)
	addiu	$3,$3,%lo($LC13)
	swl	$4,2384($2)
	lw	$5,4($3)
	swr	$4,2387($2)
	swl	$5,2388($2)
	lw	$4,8($3)
	swr	$5,2391($2)
	swl	$4,2392($2)
	lw	$5,12($3)
	swr	$4,2395($2)
	swl	$5,2396($2)
	lw	$3,16($3)
	swr	$5,2399($2)
	lw	$25,%call16(system)($28)
	swl	$3,2400($2)
	swr	$3,2403($2)
	jalr	$25
	addiu	$4,$sp,2440

	lbu	$2,46($16)
	lbu	$6,44($16)
	lbu	$7,45($16)
	sw	$2,16($sp)
	lbu	$4,1($20)
	lbu	$5,2($20)
	lbu	$8,3($20)
	lbu	$2,47($16)
	lbu	$3,2764($sp)
	sw	$4,28($sp)
	sw	$3,24($sp)
	sw	$5,32($sp)
	sw	$2,20($sp)
	sw	$8,36($sp)
	lw	$2,4($16)
	lw	$28,48($sp)
	xori	$2,$2,0x1
	sltu	$2,$2,1
	sw	$2,40($sp)
	lw	$4,%got($LC12)($28)
	lw	$2,8($16)
	lw	$25,%call16(sprintf)($28)
	addiu	$20,$sp,2184
	addiu	$5,$4,%lo($LC12)
	sw	$2,44($sp)
	jalr	$25
	move	$4,$20

	lw	$3,8($16)
	sll	$18,$2,24
	lw	$28,48($sp)
	sra	$18,$18,24
	blez	$3,$L365
	move	$19,$0

	move	$17,$16
$L326:
	lbu	$2,14($17)
	lbu	$6,12($17)
	lbu	$7,13($17)
	sw	$2,16($sp)
	lbu	$2,15($17)
	lw	$5,%got($LC8)($28)
	lw	$25,%call16(sprintf)($28)
	addu	$4,$20,$18
	sw	$2,20($sp)
	jalr	$25
	addiu	$5,$5,%lo($LC8)

	addiu	$4,$19,1
	lw	$3,8($16)
	sll	$19,$4,24
	sra	$19,$19,24
	addu	$2,$18,$2
	sll	$18,$2,24
	slt	$3,$19,$3
	lw	$28,48($sp)
	sra	$18,$18,24
	bne	$3,$0,$L326
	addiu	$17,$17,4

$L365:
	lw	$3,%got($LC14)($28)
	addu	$2,$fp,$18
	lw	$4,%lo($LC14)($3)
	addiu	$3,$3,%lo($LC14)
	swl	$4,2128($2)
	lw	$5,4($3)
	swr	$4,2131($2)
	swl	$5,2132($2)
	lw	$4,8($3)
	swr	$5,2135($2)
	swl	$4,2136($2)
	lw	$5,12($3)
	swr	$4,2139($2)
	swl	$5,2140($2)
	lw	$4,16($3)
	swr	$5,2143($2)
	lbu	$5,20($3)
	swl	$4,2144($2)
	lbu	$6,21($3)
	swr	$4,2147($2)
	sb	$5,2148($2)
	lbu	$3,22($3)
	lw	$25,%call16(system)($28)
	sb	$6,2149($2)
	sb	$3,2150($2)
	addiu	$4,$sp,2184
$L375:
	jalr	$25
	nop

$L376:
	lw	$28,48($sp)
$L129:
	lw	$5,2768($sp)
	lhu	$2,2($23)
	addiu	$3,$5,1
	lw	$4,2772($sp)
	andi	$3,$3,0xffff
	sll	$2,$2,2
	sw	$3,2768($sp)
	addu	$2,$23,$2
	sltu	$3,$3,$4
	bne	$3,$0,$L368
	addiu	$23,$2,8

$L126:
	lw	$31,2820($sp)
	lw	$fp,2816($sp)
	lw	$23,2812($sp)
	lw	$22,2808($sp)
	lw	$21,2804($sp)
	lw	$20,2800($sp)
	lw	$19,2796($sp)
	lw	$18,2792($sp)
	lw	$17,2788($sp)
	lw	$16,2784($sp)
	j	$31
	addiu	$sp,$sp,2824

	.set	macro
	.set	reorder
	.end	processV3report

	.comm	ZEROSET,36,4
	.ident	"GCC: (GNU) 3.4.4 (OpenWrt-2.0)"
