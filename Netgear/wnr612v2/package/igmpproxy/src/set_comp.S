	.file	1 "set_comp.c"
	.section .mdebug.abi32
	.previous
	.abicalls
	.text
	.align	2
	.globl	set_assign
	.ent	set_assign
	.type	set_assign, @function
set_assign:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	beq	$4,$0,$L5
	nop

	beq	$5,$0,$L5
	move	$7,$5

	addiu	$8,$5,32
$L4:
	lw	$2,0($7)
	lw	$3,4($7)
	lw	$4,8($7)
	lw	$5,12($7)
	addiu	$7,$7,16
	sw	$2,0($6)
	sw	$3,4($6)
	sw	$4,8($6)
	sw	$5,12($6)
	bne	$7,$8,$L4
	addiu	$6,$6,16

	lw	$2,0($7)
	nop
	sw	$2,0($6)
$L5:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	set_assign
	.align	2
	.globl	set_add
	.ent	set_add
	.type	set_add, @function
set_add:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	beq	$7,$0,$L7
	move	$12,$4

	move	$12,$5
	move	$5,$4
$L7:
	lw	$2,0($12)
	nop
	slt	$3,$2,9
	bne	$3,$0,$L9
	move	$10,$2

	move	$10,$0
$L9:
	lw	$2,0($5)
	nop
	slt	$3,$2,9
	bne	$3,$0,$L11
	move	$13,$2

	move	$13,$0
$L11:
	slt	$15,$0,$10
	beq	$15,$0,$L29
	move	$7,$0

$L15:
	sll	$3,$7,2
	addu	$2,$3,$12
	lw	$4,4($2)
	addiu	$7,$7,1
	addu	$3,$3,$6
	slt	$2,$7,$10
	bne	$2,$0,$L15
	sw	$4,4($3)

$L29:
	move	$7,$10
	blez	$13,$L17
	move	$14,$0

	sll	$2,$10,2
	addu	$2,$2,$6
	addiu	$5,$5,4
	addiu	$8,$2,4
$L27:
	slt	$2,$7,8
	beq	$2,$0,$L17
	move	$9,$0

	beq	$15,$0,$L33
	nop

	lw	$11,0($5)
	addiu	$4,$12,4
	move	$3,$10
$L24:
	lw	$2,0($4)
	addiu	$3,$3,-1
	bne	$11,$2,$L22
	addiu	$4,$4,4

	addiu	$9,$9,1
$L22:
	bne	$3,$0,$L24
	nop

	bne	$9,$0,$L18
	nop

$L33:
	lw	$2,0($5)
	addiu	$7,$7,1
	sw	$2,0($8)
	addiu	$8,$8,4
$L18:
	addiu	$14,$14,1
	slt	$2,$14,$13
	bne	$2,$0,$L27
	addiu	$5,$5,4

$L17:
	j	$31
	sw	$7,0($6)

	.set	macro
	.set	reorder
	.end	set_add
	.align	2
	.globl	set_subtract
	.ent	set_subtract
	.type	set_subtract, @function
set_subtract:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	beq	$7,$0,$L35
	move	$13,$0

	move	$2,$4
	move	$4,$5
	move	$5,$2
$L35:
	lw	$2,0($4)
	nop
	slt	$3,$2,9
	bne	$3,$0,$L37
	move	$7,$2

	move	$7,$0
$L37:
	lw	$2,0($5)
	nop
	slt	$3,$2,9
	bne	$3,$0,$L39
	move	$11,$2

	move	$11,$0
$L39:
	blez	$7,$L51
	nop

	addiu	$4,$4,4
	move	$9,$7
	slt	$14,$0,$11
	addiu	$8,$6,4
$L49:
	beq	$14,$0,$L54
	move	$10,$0

	lw	$12,0($4)
	addiu	$7,$5,4
	move	$3,$11
$L47:
	lw	$2,0($7)
	addiu	$3,$3,-1
	bne	$2,$12,$L45
	addiu	$7,$7,4

	addiu	$10,$10,1
$L45:
	bne	$3,$0,$L47
	nop

	bne	$10,$0,$L42
	nop

$L54:
	lw	$2,0($4)
	addiu	$13,$13,1
	sw	$2,0($8)
	addiu	$8,$8,4
$L42:
	addiu	$9,$9,-1
	bne	$9,$0,$L49
	addiu	$4,$4,4

$L51:
	j	$31
	sw	$13,0($6)

	.set	macro
	.set	reorder
	.end	set_subtract
	.align	2
	.globl	set_intersection
	.ent	set_intersection
	.type	set_intersection, @function
set_intersection:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lw	$2,0($4)
	move	$13,$0
	slt	$3,$2,9
	bne	$3,$0,$L57
	move	$7,$2

	move	$7,$0
$L57:
	lw	$2,0($5)
	nop
	slt	$3,$2,9
	bne	$3,$0,$L59
	move	$11,$2

	move	$11,$0
$L59:
	blez	$7,$L71
	nop

	addiu	$4,$4,4
	move	$9,$7
	slt	$14,$0,$11
	addiu	$8,$6,4
$L69:
	beq	$14,$0,$L62
	move	$10,$0

	lw	$12,0($4)
	addiu	$7,$5,4
	move	$3,$11
$L67:
	lw	$2,0($7)
	addiu	$3,$3,-1
	bne	$2,$12,$L65
	addiu	$7,$7,4

	addiu	$10,$10,1
$L65:
	bne	$3,$0,$L67
	nop

	beq	$10,$0,$L62
	nop

	lw	$2,0($4)
	addiu	$13,$13,1
	sw	$2,0($8)
	addiu	$8,$8,4
$L62:
	addiu	$9,$9,-1
	bne	$9,$0,$L69
	addiu	$4,$4,4

$L71:
	j	$31
	sw	$13,0($6)

	.set	macro
	.set	reorder
	.end	set_intersection
	.align	2
	.globl	set_comp
	.ent	set_comp
	.type	set_comp, @function
set_comp:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lw	$2,0($4)
	nop
	slt	$3,$2,9
	bne	$3,$0,$L76
	move	$7,$2

	move	$7,$0
$L76:
	lw	$2,0($5)
	nop
	slt	$3,$2,9
	bne	$3,$0,$L78
	nop

	move	$2,$0
$L78:
	bne	$7,$2,$L74
	li	$3,1			# 0x1

	b	$L79
	slt	$10,$0,$7

$L94:
	b	$L74
	li	$3,1			# 0x1

$L79:
	beq	$10,$0,$L91
	move	$9,$0

	addiu	$4,$4,4
$L89:
	beq	$10,$0,$L94
	li	$11,1			# 0x1

	lw	$8,0($4)
	addiu	$6,$5,4
	move	$3,$7
$L87:
	lw	$2,0($6)
	addiu	$3,$3,-1
	bne	$8,$2,$L85
	addiu	$6,$6,4

	move	$11,$0
$L85:
	bne	$3,$0,$L87
	nop

	bne	$11,$0,$L94
	nop

	addiu	$9,$9,1
	slt	$2,$9,$7
	bne	$2,$0,$L89
	addiu	$4,$4,4

$L91:
	move	$3,$0
$L74:
	j	$31
	move	$2,$3

	.set	macro
	.set	reorder
	.end	set_comp
	.ident	"GCC: (GNU) 3.4.4 (OpenWrt-2.0)"
