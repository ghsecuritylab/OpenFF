	.file	1 "br_igmpv3.c"
	.section .mdebug.abi32
	.previous
#APP
		.macro	_ssnop					
		sll	$0, $2, 1				
		.endm						
								
		.macro	_ehb					
		sll	$0, $0, 3				
		.endm						
	
		.macro	irq_enable_hazard			
		_ehb						
		.endm						
								
		.macro	irq_disable_hazard			
		_ehb						
		.endm						
								
		.macro	back_to_back_c0_hazard			
		_ehb						
		.endm
		.macro	local_irq_enable				
	.set	push						
	.set	reorder						
	.set	noat						
	ei							
	irq_enable_hazard					
	.set	pop						
	.endm
		.macro	local_irq_disable
	.set	push						
	.set	noat						
	di							
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	local_save_flags flags				
	.set	push						
	.set	reorder						
	mfc0	\flags, $12					
	.set	pop						
	.endm							

		.macro	local_irq_save result				
	.set	push						
	.set	reorder						
	.set	noat						
	di	\result					
	andi	\result, 1					
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	local_irq_restore flags				
	.set	noreorder					
	.set	noat						
	beqz	\flags, 1f					
	 di							
	ei							
1:								
	irq_disable_hazard					
	.set	at						
	.set	reorder						
	.endm							

#NO_APP
	.globl	igmpsnoopenabled
	.section	.bss
	.align	2
	.type	igmpsnoopenabled, @object
	.size	igmpsnoopenabled, 4
igmpsnoopenabled:
	.space	4
	.globl	wl_mbr_head
	.align	2
	.type	wl_mbr_head, @object
	.size	wl_mbr_head, 4
wl_mbr_head:
	.space	4
	.globl	iptv_igmp_timer
	.align	2
	.type	iptv_igmp_timer, @object
	.size	iptv_igmp_timer, 8
iptv_igmp_timer:
	.space	8
	.globl	iptv_mcast_set_entry
	.align	2
	.type	iptv_mcast_set_entry, @object
	.size	iptv_mcast_set_entry, 4
iptv_mcast_set_entry:
	.space	4
	.globl	wl_mcast_set_entry
	.align	2
	.type	wl_mcast_set_entry, @object
	.size	wl_mcast_set_entry, 4
wl_mcast_set_entry:
	.space	4
	.globl	wl_mcast_entry
	.align	2
	.type	wl_mcast_entry, @object
	.size	wl_mcast_entry, 4
wl_mcast_entry:
	.space	4
	.globl	procigmp
	.align	2
	.type	procigmp, @object
	.size	procigmp, 4
procigmp:
	.space	4
	.globl	mcast_set_entry
	.align	2
	.type	mcast_set_entry, @object
	.size	mcast_set_entry, 4
mcast_set_entry:
	.space	4
	.globl	mgroup_info
	.align	2
	.type	mgroup_info, @object
	.size	mgroup_info, 64
mgroup_info:
	.space	64
	.globl	mcache
	.align	2
	.type	mcache, @object
	.size	mcache, 64
mcache:
	.space	64
	.globl	iptvGroup
	.align	2
	.type	iptvGroup, @object
	.size	iptvGroup, 64
iptvGroup:
	.space	64
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC0:
	.ascii	"Add group[%d.%d.%d.%d] of group info.\n\000"
	.text
	.align	2
	.globl	add_group_info
	.ent	add_group_info
	.type	add_group_info, @function
add_group_info:
	.frame	$sp,48,$31		# vars= 8, regs= 3/0, args= 24, gp= 0
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
$L5:
	addiu	$sp,$sp,-48
	sw	$4,24($sp)
	lbu	$5,26($sp)
	lbu	$3,25($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	sw	$31,40($sp)
	lbu	$6,27($sp)
	sw	$4,48($sp)
	lui	$2,%hi(malloc_sizes+52)
	lw	$4,%lo(malloc_sizes+52)($2)
	addu	$3,$3,$5
	addu	$3,$3,$6
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	andi	$17,$3,0xf

	move	$16,$2
	beq	$16,$0,$L1
	move	$2,$0

	move	$4,$16
	move	$5,$0
	jal	memset
	li	$6,132			# 0x84

	lw	$2,48($sp)
	li	$5,-1			# 0xffffffffffffffff
	li	$4,1			# 0x1
	sw	$2,0($16)
	li	$3,2			# 0x2
	move	$2,$16
$L44:
	addiu	$3,$3,-1
	sw	$5,8($2)
	sw	$4,12($2)
	bgez	$3,$L44
	addiu	$2,$2,40

	lui	$2,%hi(mgroup_info)
	addiu	$2,$2,%lo(mgroup_info)
	sll	$3,$17,2
	addu	$3,$3,$2
	sw	$0,128($16)
	lw	$2,0($3)
	beq	$2,$0,$L45
	nop

	move	$3,$2
$L56:
	lw	$2,128($3)
	beq	$2,$0,$L55
	nop

	j	$L56
	lw	$3,128($3)

$L55:
	j	$L50
	sw	$16,128($3)

$L45:
	sw	$16,0($3)
$L50:
	lbu	$2,51($sp)
	lbu	$5,48($sp)
	lbu	$6,49($sp)
	lbu	$7,50($sp)
	lui	$4,%hi($LC0)
	addiu	$4,$4,%lo($LC0)
	jal	printk
	sw	$2,16($sp)

	move	$2,$16
$L1:
	lw	$31,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
	.end	add_group_info
	.align	2
	.globl	find_group_info
	.ent	find_group_info
	.type	find_group_info, @function
find_group_info:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$4,0($sp)
	lbu	$2,2($sp)
	lbu	$3,1($sp)
	move	$5,$4
	lbu	$4,3($sp)
	addu	$3,$3,$2
	lui	$2,%hi(mgroup_info)
	addu	$3,$3,$4
	andi	$3,$3,0xf
	sll	$3,$3,2
	addiu	$2,$2,%lo(mgroup_info)
	addu	$3,$3,$2
	lw	$3,0($3)
$L65:
	beq	$3,$0,$L64
	move	$2,$0

	lw	$2,0($3)
	beq	$2,$5,$L57
	move	$2,$3

	j	$L65
	lw	$3,128($3)

$L64:
$L57:
	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	find_group_info
	.section	.rodata.str1.4
	.align	2
$LC1:
	.ascii	"Remove group[%d.%d.%d.%d] of group info.\n\000"
	.align	2
$LC2:
	.ascii	"Remove group [%d.%d.%d.%d] server cache[%d.%d.%d.%d]\n\000"
	.text
	.align	2
	.globl	del_group_info
	.ent	del_group_info
	.type	del_group_info, @function
del_group_info:
	.frame	$sp,88,$31		# vars= 16, regs= 7/0, args= 40, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	sw	$4,40($sp)
	lbu	$2,41($sp)
	lbu	$3,42($sp)
	lbu	$5,43($sp)
	sw	$20,72($sp)
	addu	$2,$2,$3
	addu	$2,$2,$5
	andi	$2,$2,0xf
	lui	$20,%hi(mgroup_info)
	sw	$18,64($sp)
	sll	$18,$2,2
	addiu	$2,$20,%lo(mgroup_info)
	sw	$19,68($sp)
	sw	$31,80($sp)
	sw	$21,76($sp)
	sw	$17,60($sp)
	sw	$16,56($sp)
	addu	$2,$18,$2
	lw	$17,0($2)
	move	$6,$4
	sw	$4,88($sp)
	move	$19,$0
$L96:
	beq	$17,$0,$L72
	nop

	lw	$2,0($17)
	beq	$2,$6,$L69
	nop

	move	$19,$17
	j	$L96
	lw	$17,128($17)

$L69:
	beq	$17,$0,$L98
	lw	$6,88($sp)

	lw	$4,4($17)
	beq	$4,$0,$L99
	addiu	$2,$20,%lo(mgroup_info)

$L75:
	jal	kfree
	lw	$16,56($4)

	bne	$16,$0,$L75
	move	$4,$16

	addiu	$2,$20,%lo(mgroup_info)
$L99:
	addu	$4,$18,$2
	lw	$2,0($4)
	bne	$17,$2,$L76
	lw	$3,128($17)

	j	$L77
	sw	$3,0($4)

$L76:
	sw	$3,128($19)
$L77:
	lbu	$2,91($sp)
	lbu	$5,88($sp)
	lbu	$6,89($sp)
	lbu	$7,90($sp)
	lui	$4,%hi($LC1)
	addiu	$4,$4,%lo($LC1)
	jal	printk
	sw	$2,16($sp)

	jal	kfree
	move	$4,$17

$L72:
	lw	$6,88($sp)
$L98:
	lui	$21,%hi(mcache)
	addiu	$3,$21,%lo(mcache)
	sw	$6,44($sp)
	lbu	$2,45($sp)
	lbu	$4,46($sp)
	lbu	$5,47($sp)
	sw	$6,48($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$19,$2,2
	addu	$3,$19,$3
	lw	$18,0($3)
	move	$20,$0
$L97:
	beq	$18,$0,$L100
	lw	$31,80($sp)

	lw	$2,0($18)
	beq	$2,$6,$L80
	nop

	move	$20,$18
	j	$L97
	lw	$18,8($18)

$L80:
	beq	$18,$0,$L100
	lw	$31,80($sp)

	lw	$17,4($18)
	beq	$17,$0,$L101
	addiu	$2,$21,%lo(mcache)

$L86:
	lbu	$2,51($sp)
	lbu	$5,48($sp)
	lbu	$6,49($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,50($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	jal	printk
	sw	$2,32($sp)

	lw	$16,12($17)
	jal	kfree
	move	$4,$17

	bne	$16,$0,$L86
	move	$17,$16

	addiu	$2,$21,%lo(mcache)
$L101:
	addu	$3,$19,$2
	lw	$2,0($3)
	bne	$18,$2,$L87
	lw	$4,8($18)

	j	$L88
	sw	$4,0($3)

$L87:
	sw	$4,8($20)
$L88:
	jal	kfree
	move	$4,$18

	lw	$31,80($sp)
$L100:
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	j	$31
	addiu	$sp,$sp,88

	.set	macro
	.set	reorder
	.end	del_group_info
	.align	2
	.globl	remove_wl_all
	.ent	remove_wl_all
	.type	remove_wl_all, @function
remove_wl_all:
	.frame	$sp,24,$31		# vars= 0, regs= 2/0, args= 16, gp= 0
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-24
	sw	$16,16($sp)
	sw	$31,20($sp)
	jal	find_group_info
	nop

	beq	$2,$0,$L102
	move	$16,$2

	lw	$2,4($16)
$L109:
	beq	$2,$0,$L110
	lw	$31,20($sp)

	lw	$4,4($16)
	lw	$2,56($4)
	jal	kfree
	sw	$2,4($16)

	j	$L109
	lw	$2,4($16)

$L102:
	lw	$31,20($sp)
$L110:
	lw	$16,16($sp)
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	remove_wl_all
	.section	.rodata.str1.4
	.align	2
$LC3:
	.ascii	"group %u.%u.%u.%u\n\000"
	.align	2
$LC5:
	.ascii	"include\000"
	.align	2
$LC6:
	.ascii	"exclude\000"
	.align	2
$LC4:
	.ascii	"port%d mode %s source \000"
	.align	2
$LC7:
	.ascii	"%u.%u.%u.%u \000"
	.align	2
$LC8:
	.ascii	"\n\000"
	.align	2
$LC9:
	.ascii	"port%d mode %s source ---\n\000"
	.align	2
$LC10:
	.ascii	"port%d ---\n\000"
	.align	2
$LC11:
	.ascii	"HW entry : \n\000"
	.align	2
$LC12:
	.ascii	"group %u.%u.%u.%u : \000"
	.align	2
$LC13:
	.ascii	"NULL\000"
	.align	2
$LC14:
	.ascii	"server %u.%u.%u.%u index %d \000"
	.text
	.align	2
	.ent	br_mcast_entry_read
	.type	br_mcast_entry_read, @function
br_mcast_entry_read:
	.frame	$sp,72,$31		# vars= 0, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$fp,64($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$18,40($sp)
	sw	$31,68($sp)
	sw	$23,60($sp)
	sw	$22,56($sp)
	sw	$19,44($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	move	$20,$4
	sw	$5,76($sp)
	sw	$6,80($sp)
	move	$fp,$7
	move	$18,$0
	move	$21,$0
	lui	$2,%hi(mgroup_info)
$L171:
	sll	$3,$21,2
	addiu	$2,$2,%lo(mgroup_info)
	addu	$3,$3,$2
	lw	$16,0($3)
$L167:
	beq	$16,$0,$L156
	lui	$5,%hi($LC3)

	lbu	$2,2($16)
	lbu	$6,0($16)
	lbu	$7,1($16)
	sw	$2,16($sp)
	lbu	$2,3($16)
	addu	$4,$20,$18
	addiu	$5,$5,%lo($LC3)
	jal	sprintf
	sw	$2,20($sp)

	addu	$18,$18,$2
	move	$22,$0
	move	$23,$0
	move	$17,$16
$L136:
	lw	$2,8($17)
	blez	$2,$L120
	nop

	lw	$3,12($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L123
	addu	$4,$20,$18

	lui	$2,%hi($LC5)
	j	$L124
	addiu	$7,$2,%lo($LC5)

$L123:
	lui	$2,%hi($LC6)
	addiu	$7,$2,%lo($LC6)
$L124:
	lui	$5,%hi($LC4)
	addiu	$5,$5,%lo($LC4)
	jal	sprintf
	move	$6,$22

	lw	$3,8($17)
	addu	$18,$18,$2
	blez	$3,$L160
	move	$19,$0

$L128:
	sll	$2,$23,1
	addu	$2,$2,$19
	sll	$2,$2,2
	addu	$2,$2,$16
	lbu	$3,18($2)
	lbu	$6,16($2)
	lbu	$7,17($2)
	sw	$3,16($sp)
	lbu	$2,19($2)
	lui	$5,%hi($LC7)
	addu	$4,$20,$18
	addiu	$5,$5,%lo($LC7)
	jal	sprintf
	sw	$2,20($sp)

	lw	$3,8($17)
	addiu	$19,$19,1
	slt	$3,$19,$3
	bne	$3,$0,$L128
	addu	$18,$18,$2

$L160:
	lui	$5,%hi($LC8)
	addiu	$5,$5,%lo($LC8)
	jal	sprintf
	addu	$4,$20,$18

	j	$L170
	addiu	$22,$22,1

$L120:
	bne	$2,$0,$L130
	addu	$4,$20,$18

	lw	$3,12($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L133
	nop

	lui	$2,%hi($LC5)
	j	$L134
	addiu	$7,$2,%lo($LC5)

$L133:
	lui	$2,%hi($LC6)
	addiu	$7,$2,%lo($LC6)
$L134:
	lui	$5,%hi($LC9)
	addiu	$5,$5,%lo($LC9)
	jal	sprintf
	move	$6,$22

	j	$L170
	addiu	$22,$22,1

$L130:
	lui	$5,%hi($LC10)
	addiu	$5,$5,%lo($LC10)
	jal	sprintf
	move	$6,$22

	addiu	$22,$22,1
$L170:
	addu	$18,$18,$2
	slt	$2,$22,3
	addiu	$17,$17,40
	bne	$2,$0,$L136
	addiu	$23,$23,5

	j	$L167
	lw	$16,128($16)

$L156:
	addiu	$21,$21,1
	slt	$2,$21,16
	bne	$2,$0,$L171
	lui	$2,%hi(mgroup_info)

	lui	$5,%hi($LC11)
	addu	$4,$20,$18
	jal	sprintf
	addiu	$5,$5,%lo($LC11)

	addu	$18,$18,$2
	move	$21,$0
	lui	$2,%hi(mcache)
$L172:
	sll	$3,$21,2
	addiu	$2,$2,%lo(mcache)
	addu	$3,$3,$2
	lw	$17,0($3)
$L169:
	beq	$17,$0,$L164
	lui	$5,%hi($LC12)

	lbu	$2,2($17)
	lbu	$6,0($17)
	lbu	$7,1($17)
	sw	$2,16($sp)
	lbu	$2,3($17)
	addu	$4,$20,$18
	addiu	$5,$5,%lo($LC12)
	jal	sprintf
	sw	$2,20($sp)

	lw	$16,4($17)
	bne	$16,$0,$L147
	addu	$18,$18,$2

	lui	$5,%hi($LC13)
	addu	$4,$20,$18
	jal	sprintf
	addiu	$5,$5,%lo($LC13)

	j	$L166
	addu	$18,$18,$2

$L147:
	lbu	$2,2($16)
	lbu	$6,0($16)
	lbu	$7,1($16)
	sw	$2,16($sp)
	lbu	$2,3($16)
	lui	$5,%hi($LC14)
	addu	$4,$20,$18
	sw	$2,20($sp)
	lw	$2,4($16)
	addiu	$5,$5,%lo($LC14)
	jal	sprintf
	sw	$2,24($sp)

	lw	$16,12($16)
	bne	$16,$0,$L147
	addu	$18,$18,$2

$L166:
	lui	$5,%hi($LC8)
	addu	$4,$20,$18
	jal	sprintf
	addiu	$5,$5,%lo($LC8)

	lw	$17,8($17)
	j	$L169
	addu	$18,$18,$2

$L164:
	addiu	$21,$21,1
	slt	$2,$21,16
	bne	$2,$0,$L172
	lui	$2,%hi(mcache)

	lw	$3,80($sp)
	addu	$2,$3,$fp
	slt	$2,$2,$18
	bne	$2,$0,$L173
	lw	$4,80($sp)

	lw	$2,88($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	lw	$4,80($sp)
$L173:
	lw	$31,68($sp)
	lw	$23,60($sp)
	subu	$18,$18,$4
	slt	$2,$fp,$18
	movn	$18,$fp,$2
	addu	$2,$20,$4
	lw	$4,76($sp)
	slt	$3,$18,0
	lw	$fp,64($sp)
	sw	$2,0($4)
	move	$2,$0
	movz	$2,$18,$3
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
	.end	br_mcast_entry_read
	.section	.rodata.str1.4
	.align	2
$LC15:
	.ascii	"clean\000"
	.align	2
$LC16:
	.ascii	"remove\000"
	.align	2
$LC18:
	.ascii	" \000"
	.align	2
$LC17:
	.ascii	"%u.%u.%u.%u\000"
	.text
	.align	2
	.ent	br_mcast_entry_write
	.type	br_mcast_entry_write, @function
br_mcast_entry_write:
	.frame	$sp,424,$31		# vars= 344, regs= 10/0, args= 40, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-424
	addiu	$7,$sp,40
	sltu	$2,$6,257
	sw	$23,412($sp)
	sw	$31,420($sp)
	sw	$fp,416($sp)
	sw	$22,408($sp)
	sw	$21,404($sp)
	sw	$20,400($sp)
	sw	$19,396($sp)
	sw	$18,392($sp)
	sw	$17,388($sp)
	sw	$16,384($sp)
	move	$23,$6
	sw	$7,312($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	li	$8,-28			# 0xffffffffffffffe4
	.set	macro
	.set	reorder

	addu	$2,$5,$6
	lw	$3,24($28)
	or	$2,$5,$2
	or	$2,$2,$6
	and	$3,$3,$2
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L179
	move	$2,$6
	.set	macro
	.set	reorder

	move	$4,$7
#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
	move	$2,$6
$L179:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L174
	li	$8,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addu	$2,$7,$23
	lui	$3,%hi($LC15)
	sb	$0,0($2)
	addiu	$3,$3,%lo($LC15)
	li	$2,5			# 0x5
	move	$5,$7
#APP
	.set	noreorder
	.set	noat
1:	lbu	$4,($5)
	beqz	$2,2f
	lbu	$1,($3)
	subu	$2,1
	bne	$1,$4,3f
	addiu	$5,1
	bnez	$4,1b
	addiu	$3,1
2:
	move	$4,$1
3:	subu	$4,$1
	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L183
	lui	$3,%hi($LC16)
	.set	macro
	.set	reorder

	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	move	$19,$0
$L199:
	lui	$21,%hi(mgroup_info)
	sll	$20,$19,2
	addiu	$2,$21,%lo(mgroup_info)
	addu	$2,$20,$2
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L343
	addiu	$2,$21,%lo(mgroup_info)
	.set	macro
	.set	reorder

$L198:
	lw	$4,4($17)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L313
	lw	$18,128($17)
	.set	macro
	.set	reorder

$L197:
	.set	noreorder
	.set	nomacro
	jal	kfree
	lw	$16,56($4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L197
	move	$4,$16
	.set	macro
	.set	reorder

$L313:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$18,$0,$L198
	move	$17,$18
	.set	macro
	.set	reorder

	addiu	$2,$21,%lo(mgroup_info)
$L343:
	addiu	$19,$19,1
	addu	$2,$20,$2
	slt	$3,$19,16
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L199
	sw	$0,0($2)
	.set	macro
	.set	reorder

	move	$18,$0
$L210:
	lui	$20,%hi(mcache)
	sll	$19,$18,2
	addiu	$2,$20,%lo(mcache)
	addu	$2,$19,$2
	lw	$17,0($2)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L344
	addiu	$2,$20,%lo(mcache)
	.set	macro
	.set	reorder

$L209:
	lw	$4,4($17)
	beq	$4,$0,$L319
$L208:
	.set	noreorder
	.set	nomacro
	jal	kfree
	lw	$16,12($4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L208
	move	$4,$16
	.set	macro
	.set	reorder

$L319:
	lw	$16,8($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L209
	move	$17,$16
	.set	macro
	.set	reorder

	addiu	$2,$20,%lo(mcache)
$L344:
	addiu	$18,$18,1
	addu	$2,$19,$2
	slt	$3,$18,16
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L210
	sw	$0,0($2)
	.set	macro
	.set	reorder

	lui	$17,%hi(wl_mbr_head)
	lw	$4,%lo(wl_mbr_head)($17)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L345
	lui	$2,%hi(mac_cache)
	.set	macro
	.set	reorder

$L214:
	.set	noreorder
	.set	nomacro
	jal	kfree
	lw	$16,56($4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L214
	move	$4,$16
	.set	macro
	.set	reorder

	lui	$2,%hi(mac_cache)
$L345:
	sw	$0,%lo(wl_mbr_head)($17)
	addiu	$2,$2,%lo(mac_cache)
	li	$3,9			# 0x9
$L218:
	addiu	$3,$3,-1
	sb	$0,0($2)
	.set	noreorder
	.set	nomacro
	bgez	$3,$L218
	addiu	$2,$2,16
	.set	macro
	.set	reorder

	j	$L307
$L183:
	addiu	$3,$3,%lo($LC16)
	li	$2,6			# 0x6
#APP
	.set	noreorder
	.set	noat
1:	lbu	$4,($7)
	beqz	$2,2f
	lbu	$1,($3)
	subu	$2,1
	bne	$1,$4,3f
	addiu	$7,1
	bnez	$4,1b
	addiu	$3,1
2:
	move	$4,$1
3:	subu	$4,$1
	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L222
	lui	$fp,%hi($LC18)
	.set	macro
	.set	reorder

	lw	$4,312($sp)
	addiu	$2,$sp,296
	lui	$5,%hi($LC17)
	sw	$2,16($sp)
	addiu	$5,$5,%lo($LC17)
	addiu	$2,$sp,300
	addiu	$4,$4,7
	addiu	$6,$sp,304
	addiu	$7,$sp,308
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,304($sp)
	lw	$2,308($sp)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,296($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,300($sp)
	or	$16,$3,$2
	li	$2,-268435456			# 0xfffffffff0000000
	and	$2,$16,$2
	li	$3,-536870912			# 0xffffffffe0000000
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	.set	noreorder
	.set	nomacro
	jal	find_group_info
	move	$4,$16
	.set	macro
	.set	reorder

	beq	$2,$0,$L307
	.set	noreorder
	.set	nomacro
	jal	del_group_info
	move	$4,$16
	.set	macro
	.set	reorder

	j	$L307
$L222:
	addiu	$4,$sp,312
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$fp,%lo($LC18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	lb	$4,0($2)
	li	$2,97			# 0x61
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L236
	li	$22,1			# 0x1
	.set	macro
	.set	reorder

	li	$2,100			# 0x64
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L174
	move	$22,$0
	.set	macro
	.set	reorder

$L236:
	addiu	$4,$sp,312
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$fp,%lo($LC18)
	.set	macro
	.set	reorder

	lui	$17,%hi($LC18)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	move	$4,$2
	lui	$2,%hi($LC17)
	addiu	$5,$2,%lo($LC17)
	addiu	$2,$sp,316
	sw	$2,16($sp)
	addiu	$6,$sp,324
	addiu	$2,$sp,320
	addiu	$7,$sp,328
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,324($sp)
	lw	$2,328($sp)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,316($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,320($sp)
	or	$16,$3,$2
	li	$2,-268435456			# 0xfffffffff0000000
	and	$2,$16,$2
	li	$3,-536870912			# 0xffffffffe0000000
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	addiu	$4,$sp,312
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$17,%lo($LC18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	lb	$2,0($2)
	addiu	$19,$2,-48
	slt	$2,$19,3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	addiu	$4,$sp,312
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$17,%lo($LC18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	lb	$2,0($2)
	addiu	$21,$2,-48
	slt	$2,$21,9
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	addiu	$5,$17,%lo($LC18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$4,$sp,312
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	lb	$4,0($2)
	li	$2,49			# 0x31
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L248
	li	$18,1			# 0x1
	.set	macro
	.set	reorder

	li	$2,48			# 0x30
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L174
	move	$18,$0
	.set	macro
	.set	reorder

$L248:
	li	$2,536870912			# 0x20000000
	addu	$2,$16,$2
	sltu	$2,$2,256
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L339
	li	$2,-268500992			# 0xffffffffefff0000
	.set	macro
	.set	reorder

	ori	$2,$2,0xfffa
	.set	noreorder
	.set	nomacro
	beq	$16,$2,$L174
	move	$8,$23
	.set	macro
	.set	reorder

	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	li	$20,1			# 0x1
	bne	$22,$20,$L261
	.set	noreorder
	.set	nomacro
	jal	find_group_info
	move	$4,$16
	.set	macro
	.set	reorder

	move	$17,$2
	sll	$22,$19,2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L262
	slt	$20,$0,$21
	.set	macro
	.set	reorder

	addu	$2,$22,$19
	sll	$2,$2,3
	addu	$2,$2,$17
	sw	$18,12($2)
	sw	$21,8($2)
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L271
	move	$18,$0
	.set	macro
	.set	reorder

$L270:
	addiu	$4,$sp,312
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$fp,%lo($LC18)
	.set	macro
	.set	reorder

	move	$3,$2
	addu	$2,$22,$19
	sll	$2,$2,1
	addu	$2,$2,$18
	lui	$6,%hi($LC17)
	sll	$2,$2,2
	addiu	$5,$6,%lo($LC17)
	move	$4,$3
	addiu	$6,$sp,340
	addiu	$7,$sp,344
	addu	$20,$2,$17
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L338
	addiu	$18,$18,1
	.set	macro
	.set	reorder

	addiu	$2,$sp,332
	sw	$2,16($sp)
	addiu	$2,$sp,336
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,340($sp)
	lw	$2,344($sp)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,332($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,336($sp)
	or	$3,$3,$2
	slt	$2,$18,$21
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L270
	sw	$3,16($20)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L341
	sw	$16,364($sp)
	.set	macro
	.set	reorder

$L262:
	.set	noreorder
	.set	nomacro
	jal	add_group_info
	move	$4,$16
	.set	macro
	.set	reorder

	move	$17,$2
	addu	$2,$22,$19
	sll	$2,$2,3
	addu	$2,$2,$17
	sw	$18,12($2)
	sw	$21,8($2)
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L271
	move	$18,$0
	.set	macro
	.set	reorder

$L279:
	addiu	$4,$sp,312
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$fp,%lo($LC18)
	.set	macro
	.set	reorder

	move	$3,$2
	addu	$2,$22,$19
	sll	$2,$2,1
	addu	$2,$2,$18
	lui	$6,%hi($LC17)
	sll	$2,$2,2
	addiu	$5,$6,%lo($LC17)
	move	$4,$3
	addiu	$6,$sp,356
	addiu	$7,$sp,360
	addu	$20,$2,$17
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L334
	addiu	$18,$18,1
	.set	macro
	.set	reorder

	addiu	$2,$sp,348
	sw	$2,16($sp)
	addiu	$2,$sp,352
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,356($sp)
	lw	$2,360($sp)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,348($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,352($sp)
	or	$3,$3,$2
	slt	$2,$18,$21
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L279
	sw	$3,16($20)
	.set	macro
	.set	reorder

$L271:
	sw	$16,364($sp)
$L341:
	lbu	$2,365($sp)
	lbu	$3,366($sp)
	lbu	$4,367($sp)
	lui	$21,%hi(mcache)
	addu	$2,$2,$3
	addu	$2,$2,$4
	andi	$2,$2,0xf
	sll	$19,$2,2
	addiu	$2,$21,%lo(mcache)
	addu	$2,$19,$2
	lw	$18,0($2)
	move	$20,$0
	sw	$16,368($sp)
$L336:
	beq	$18,$0,$L307
	lw	$2,0($18)
	beq	$2,$16,$L282
	move	$20,$18
	.set	noreorder
	.set	nomacro
	j	$L336
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L282:
	beq	$18,$0,$L307
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L342
	addiu	$2,$21,%lo(mcache)
	.set	macro
	.set	reorder

$L288:
	lbu	$2,371($sp)
	lbu	$5,368($sp)
	lbu	$6,369($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,370($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L288
	move	$17,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L342
	addiu	$2,$21,%lo(mcache)
	.set	macro
	.set	reorder

$L261:
	.set	noreorder
	.set	nomacro
	jal	find_group_info
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L307
	sll	$4,$19,2
	.set	macro
	.set	reorder

	addu	$4,$4,$19
	sll	$4,$4,3
	addu	$4,$4,$2
	li	$2,-1			# 0xffffffffffffffff
	sw	$20,12($4)
	sw	$2,8($4)
	move	$5,$0
	addiu	$4,$4,16
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,32			# 0x20
	.set	macro
	.set	reorder

	sw	$16,372($sp)
	lbu	$2,373($sp)
	lbu	$3,374($sp)
	lbu	$4,375($sp)
	lui	$21,%hi(mcache)
	addu	$2,$2,$3
	addu	$2,$2,$4
	andi	$2,$2,0xf
	sll	$19,$2,2
	addiu	$2,$21,%lo(mcache)
	addu	$2,$19,$2
	lw	$18,0($2)
	move	$20,$0
	sw	$16,376($sp)
$L337:
	beq	$18,$0,$L307
	lw	$2,0($18)
	beq	$2,$16,$L296
	move	$20,$18
	.set	noreorder
	.set	nomacro
	j	$L337
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L296:
	beq	$18,$0,$L307
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L342
	addiu	$2,$21,%lo(mcache)
	.set	macro
	.set	reorder

$L302:
	lbu	$2,379($sp)
	lbu	$5,376($sp)
	lbu	$6,377($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,378($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L302
	move	$17,$16
	.set	macro
	.set	reorder

	addiu	$2,$21,%lo(mcache)
$L342:
	addu	$4,$19,$2
	lw	$2,0($4)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L303
	lw	$3,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L304
	sw	$3,0($4)
	.set	macro
	.set	reorder

$L334:
	.set	noreorder
	.set	nomacro
	jal	del_group_info
	move	$4,$16
	.set	macro
	.set	reorder

$L338:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	j	$L174
	li	$8,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

$L303:
	sw	$3,8($20)
$L304:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L307:
	jal	local_bh_enable
$L339:
	move	$8,$23
$L174:
	lw	$31,420($sp)
	lw	$fp,416($sp)
	lw	$23,412($sp)
	lw	$22,408($sp)
	lw	$21,404($sp)
	lw	$20,400($sp)
	lw	$19,396($sp)
	lw	$18,392($sp)
	lw	$17,388($sp)
	lw	$16,384($sp)
	move	$2,$8
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,424
	.set	macro
	.set	reorder

	.end	br_mcast_entry_write
	.section	.rodata.str1.4
	.align	2
$LC20:
	.ascii	"Wireless\000"
	.align	2
$LC21:
	.ascii	"Wire\000"
	.align	2
$LC19:
	.ascii	"member%d %s mode %s source \000"
	.align	2
$LC22:
	.ascii	"member%d %s mode %s source ---\n\000"
	.text
	.align	2
	.ent	br_iptv_mcast_entry_read
	.type	br_iptv_mcast_entry_read, @function
br_iptv_mcast_entry_read:
	.frame	$sp,72,$31		# vars= 0, regs= 10/0, args= 32, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$fp,64($sp)
	sw	$21,52($sp)
	sw	$20,48($sp)
	sw	$18,40($sp)
	sw	$31,68($sp)
	sw	$23,60($sp)
	sw	$22,56($sp)
	sw	$19,44($sp)
	sw	$17,36($sp)
	sw	$16,32($sp)
	move	$20,$4
	sw	$5,76($sp)
	sw	$6,80($sp)
	move	$fp,$7
	move	$18,$0
	move	$21,$0
	lui	$2,%hi(iptvGroup)
$L413:
	sll	$3,$21,2
	addiu	$2,$2,%lo(iptvGroup)
	addu	$3,$3,$2
	lw	$17,0($3)
$L409:
	beq	$17,$0,$L398
	lui	$5,%hi($LC3)

	lbu	$2,2($17)
	lbu	$6,0($17)
	lbu	$7,1($17)
	sw	$2,16($sp)
	lbu	$2,3($17)
	addu	$4,$20,$18
	addiu	$5,$5,%lo($LC3)
	jal	sprintf
	sw	$2,20($sp)

	addu	$18,$18,$2
	move	$22,$0
	move	$16,$17
	move	$23,$0
$L378:
	lbu	$2,10($16)
	beq	$2,$0,$L412
	nop

	lw	$2,16($16)
	blez	$2,$L355
	li	$2,128			# 0x80

	lbu	$3,11($16)
	bne	$3,$2,$L360
	addu	$4,$20,$18

	lui	$2,%hi($LC20)
	j	$L361
	addiu	$7,$2,%lo($LC20)

$L360:
	lui	$2,%hi($LC21)
	addiu	$7,$2,%lo($LC21)
$L361:
	lw	$3,20($16)
	li	$2,1			# 0x1
	bne	$3,$2,$L362
	lui	$2,%hi($LC5)

	j	$L363
	addiu	$2,$2,%lo($LC5)

$L362:
	lui	$2,%hi($LC6)
	addiu	$2,$2,%lo($LC6)
$L363:
	lui	$5,%hi($LC19)
	addiu	$5,$5,%lo($LC19)
	move	$6,$22
	jal	sprintf
	sw	$2,16($sp)

	lw	$3,16($16)
	addu	$18,$18,$2
	blez	$3,$L402
	move	$19,$0

$L367:
	sll	$2,$23,1
	addu	$2,$2,$19
	sll	$2,$2,2
	addu	$2,$2,$17
	lbu	$3,26($2)
	lbu	$6,24($2)
	lbu	$7,25($2)
	sw	$3,16($sp)
	lbu	$2,27($2)
	lui	$5,%hi($LC7)
	addu	$4,$20,$18
	addiu	$5,$5,%lo($LC7)
	jal	sprintf
	sw	$2,20($sp)

	lw	$3,16($16)
	addiu	$19,$19,1
	slt	$3,$19,$3
	bne	$3,$0,$L367
	addu	$18,$18,$2

$L402:
	lui	$5,%hi($LC8)
	addiu	$5,$5,%lo($LC8)
	jal	sprintf
	addu	$4,$20,$18

	j	$L354
	addu	$18,$18,$2

$L355:
	lbu	$2,10($16)
$L412:
	beq	$2,$0,$L354
	nop

	lw	$2,16($16)
	bne	$2,$0,$L354
	li	$2,128			# 0x80

	lbu	$3,11($16)
	bne	$3,$2,$L374
	addu	$4,$20,$18

	lui	$2,%hi($LC20)
	j	$L375
	addiu	$7,$2,%lo($LC20)

$L374:
	lui	$2,%hi($LC21)
	addiu	$7,$2,%lo($LC21)
$L375:
	lw	$3,20($16)
	li	$2,1			# 0x1
	bne	$3,$2,$L376
	lui	$2,%hi($LC5)

	j	$L377
	addiu	$2,$2,%lo($LC5)

$L376:
	lui	$2,%hi($LC6)
	addiu	$2,$2,%lo($LC6)
$L377:
	lui	$5,%hi($LC22)
	addiu	$5,$5,%lo($LC22)
	move	$6,$22
	jal	sprintf
	sw	$2,16($sp)

	addu	$18,$18,$2
$L354:
	addiu	$22,$22,1
	slt	$2,$22,9
	addiu	$23,$23,7
	bne	$2,$0,$L378
	addiu	$16,$16,56

	lui	$5,%hi($LC8)
	addu	$4,$20,$18
	lw	$17,464($17)
	jal	sprintf
	addiu	$5,$5,%lo($LC8)

	j	$L409
	addu	$18,$18,$2

$L398:
	addiu	$21,$21,1
	slt	$2,$21,16
	bne	$2,$0,$L413
	lui	$2,%hi(iptvGroup)

	lui	$5,%hi($LC11)
	addu	$4,$20,$18
	jal	sprintf
	addiu	$5,$5,%lo($LC11)

	addu	$18,$18,$2
	move	$21,$0
	lui	$2,%hi(mcache)
$L414:
	sll	$3,$21,2
	addiu	$2,$2,%lo(mcache)
	addu	$3,$3,$2
	lw	$17,0($3)
$L411:
	beq	$17,$0,$L406
	lui	$5,%hi($LC12)

	lbu	$2,2($17)
	lbu	$6,0($17)
	lbu	$7,1($17)
	sw	$2,16($sp)
	lbu	$2,3($17)
	addu	$4,$20,$18
	addiu	$5,$5,%lo($LC12)
	jal	sprintf
	sw	$2,20($sp)

	lw	$16,4($17)
	bne	$16,$0,$L389
	addu	$18,$18,$2

	lui	$5,%hi($LC13)
	addu	$4,$20,$18
	jal	sprintf
	addiu	$5,$5,%lo($LC13)

	j	$L408
	addu	$18,$18,$2

$L389:
	lbu	$2,2($16)
	lbu	$6,0($16)
	lbu	$7,1($16)
	sw	$2,16($sp)
	lbu	$2,3($16)
	lui	$5,%hi($LC14)
	addu	$4,$20,$18
	sw	$2,20($sp)
	lw	$2,4($16)
	addiu	$5,$5,%lo($LC14)
	jal	sprintf
	sw	$2,24($sp)

	lw	$16,12($16)
	bne	$16,$0,$L389
	addu	$18,$18,$2

$L408:
	lui	$5,%hi($LC8)
	addu	$4,$20,$18
	jal	sprintf
	addiu	$5,$5,%lo($LC8)

	lw	$17,8($17)
	j	$L411
	addu	$18,$18,$2

$L406:
	addiu	$21,$21,1
	slt	$2,$21,16
	bne	$2,$0,$L414
	lui	$2,%hi(mcache)

	lw	$3,80($sp)
	addu	$2,$3,$fp
	slt	$2,$2,$18
	bne	$2,$0,$L415
	lw	$4,80($sp)

	lw	$2,88($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
	lw	$4,80($sp)
$L415:
	lw	$31,68($sp)
	lw	$23,60($sp)
	subu	$18,$18,$4
	slt	$2,$fp,$18
	movn	$18,$fp,$2
	addu	$2,$20,$4
	lw	$4,76($sp)
	slt	$3,$18,0
	lw	$fp,64($sp)
	sw	$2,0($4)
	move	$2,$0
	movz	$2,$18,$3
	lw	$22,56($sp)
	lw	$21,52($sp)
	lw	$20,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
	.end	br_iptv_mcast_entry_read
	.align	2
	.ent	br_iptv_mcast_entry_write
	.type	br_iptv_mcast_entry_write, @function
br_iptv_mcast_entry_write:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	j	$31
	move	$2,$0

	.set	macro
	.set	reorder
	.end	br_iptv_mcast_entry_write
	.section	.rodata.str1.4
	.align	2
$LC23:
	.ascii	"%c\n\000"
	.text
	.align	2
	.ent	br_igmpread_proc
	.type	br_igmpread_proc, @function
br_igmpread_proc:
	.frame	$sp,40,$31		# vars= 0, regs= 5/0, args= 16, gp= 0
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-40
	sw	$19,28($sp)
	sw	$18,24($sp)
	sw	$17,20($sp)
	sw	$16,16($sp)
	sw	$31,32($sp)
	lui	$2,%hi(igmpsnoopenabled)
	lw	$2,%lo(igmpsnoopenabled)($2)
	move	$19,$5
	lui	$5,%hi($LC23)
	move	$17,$6
	addiu	$5,$5,%lo($LC23)
	addiu	$6,$2,48
	move	$16,$7
	jal	sprintf
	move	$18,$4

	subu	$4,$2,$17
	addu	$3,$17,$16
	slt	$3,$3,$2
	slt	$2,$16,$4
	movn	$4,$16,$2
	addu	$18,$18,$17
	bne	$3,$0,$L418
	slt	$5,$4,0

	lw	$2,56($sp)
	li	$3,1			# 0x1
	sw	$3,0($2)
$L418:
	sw	$18,0($19)
	lw	$31,32($sp)
	lw	$19,28($sp)
	lw	$18,24($sp)
	lw	$17,20($sp)
	lw	$16,16($sp)
	move	$2,$0
	movz	$2,$4,$5
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
	.end	br_igmpread_proc
	.align	2
	.ent	br_igmpwrite_proc
	.type	br_igmpwrite_proc, @function
br_igmpwrite_proc:
	.frame	$sp,16,$31		# vars= 8, regs= 1/0, args= 0, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-16
	sltu	$2,$6,2
	sw	$31,8($sp)
	move	$13,$6
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L421
	li	$4,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addiu	$2,$5,1
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L421
	or	$7,$5,$2
	.set	macro
	.set	reorder

	lw	$2,24($28)
	move	$4,$sp
	and	$2,$2,$7
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L423
	li	$6,1			# 0x1
	.set	macro
	.set	reorder

#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L423
	move	$4,$13
	.set	macro
	.set	reorder

	lbu	$2,0($sp)
	lui	$3,%hi(igmpsnoopenabled)
	addiu	$2,$2,-48
	.set	noreorder
	.set	nomacro
	j	$L421
	sw	$2,%lo(igmpsnoopenabled)($3)
	.set	macro
	.set	reorder

$L423:
	li	$4,-14			# 0xfffffffffffffff2
$L421:
	lw	$31,8($sp)
	move	$2,$4
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,16
	.set	macro
	.set	reorder

	.end	br_igmpwrite_proc
	.section	.rodata.str1.4
	.align	2
$LC24:
	.ascii	"Wireless mac cache:\n\000"
	.align	2
$LC25:
	.ascii	"NULL\n\000"
	.align	2
$LC26:
	.ascii	"%u.%u.%u.%u %02x:%02x:%02x:%02x:%02x:%02x from %s\n\000"
	.text
	.align	2
	.ent	wl_mcast_read
	.type	wl_mcast_read, @function
wl_mcast_read:
	.frame	$sp,88,$31		# vars= 0, regs= 7/0, args= 56, gp= 0
	.mask	0x803f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-88
	sw	$21,76($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$18,64($sp)
	sw	$17,60($sp)
	sw	$31,80($sp)
	sw	$16,56($sp)
	lui	$2,%hi(wl_mbr_head)
	lw	$16,%lo(wl_mbr_head)($2)
	move	$21,$5
	lui	$5,%hi($LC24)
	addiu	$5,$5,%lo($LC24)
	move	$20,$6
	move	$19,$7
	jal	sprintf
	move	$18,$4

	bne	$16,$0,$L434
	move	$17,$2

	lui	$5,%hi($LC25)
	addiu	$5,$5,%lo($LC25)
	jal	sprintf
	addu	$4,$18,$2

	j	$L444
	addu	$17,$17,$2

$L434:
	lbu	$2,10($16)
	lbu	$6,8($16)
	lbu	$7,9($16)
	sw	$2,16($sp)
	lbu	$2,11($16)
	lui	$5,%hi($LC26)
	addu	$4,$18,$17
	sw	$2,20($sp)
	lbu	$2,0($16)
	addiu	$5,$5,%lo($LC26)
	sw	$2,24($sp)
	lbu	$2,1($16)
	sw	$2,28($sp)
	lbu	$2,2($16)
	sw	$2,32($sp)
	lbu	$2,3($16)
	sw	$2,36($sp)
	lbu	$2,4($16)
	sw	$2,40($sp)
	lbu	$2,5($16)
	sw	$2,44($sp)
	lw	$2,52($16)
	jal	sprintf
	sw	$2,48($sp)

	lw	$16,56($16)
	bne	$16,$0,$L434
	addu	$17,$17,$2

$L444:
	lui	$5,%hi($LC8)
	addu	$4,$18,$17
	jal	sprintf
	addiu	$5,$5,%lo($LC8)

	addu	$17,$17,$2
	move	$16,$0
	lui	$2,%hi(mac_cache+8)
$L447:
	sll	$3,$16,4
	addiu	$2,$2,%lo(mac_cache+8)
	addu	$6,$3,$2
	lui	$2,%hi(mac_cache+9)
	addiu	$2,$2,%lo(mac_cache+9)
	addu	$7,$3,$2
	lui	$2,%hi(mac_cache+11)
	addiu	$2,$2,%lo(mac_cache+11)
	addu	$9,$3,$2
	lui	$2,%hi(mac_cache+10)
	addiu	$2,$2,%lo(mac_cache+10)
	addu	$8,$3,$2
	lui	$2,%hi(mac_cache)
	addiu	$2,$2,%lo(mac_cache)
	addu	$11,$3,$2
	lbu	$2,0($11)
	lui	$5,%hi($LC26)
	addiu	$16,$16,1
	addiu	$5,$5,%lo($LC26)
	beq	$2,$0,$L437
	addu	$4,$18,$17

	lbu	$12,0($8)
	lbu	$13,0($9)
	lw	$14,12($11)
	lbu	$2,2($11)
	lbu	$3,3($11)
	lbu	$8,4($11)
	lbu	$9,5($11)
	lbu	$10,6($11)
	lbu	$6,0($6)
	lbu	$7,0($7)
	lbu	$11,7($11)
	sw	$12,16($sp)
	sw	$13,20($sp)
	sw	$2,24($sp)
	sw	$3,28($sp)
	sw	$8,32($sp)
	sw	$9,36($sp)
	sw	$10,40($sp)
	sw	$11,44($sp)
	jal	sprintf
	sw	$14,48($sp)

	addu	$17,$17,$2
$L437:
	slt	$2,$16,10
	bne	$2,$0,$L447
	lui	$2,%hi(mac_cache+8)

	addu	$2,$20,$19
	slt	$2,$2,$17
	bne	$2,$0,$L440
	lw	$2,104($sp)

	li	$3,1			# 0x1
	sw	$3,0($2)
$L440:
	subu	$17,$17,$20
	slt	$2,$19,$17
	movn	$17,$19,$2
	addu	$2,$18,$20
	slt	$3,$17,0
	sw	$2,0($21)
	move	$2,$0
	movz	$2,$17,$3
	lw	$31,80($sp)
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	j	$31
	addiu	$sp,$sp,88

	.set	macro
	.set	reorder
	.end	wl_mcast_read
	.align	2
	.ent	wl_mcast_write
	.type	wl_mcast_write, @function
wl_mcast_write:
	.frame	$sp,384,$31		# vars= 304, regs= 10/0, args= 40, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-384
	sltu	$2,$6,257
	sw	$fp,376($sp)
	sw	$31,380($sp)
	sw	$23,372($sp)
	sw	$22,368($sp)
	sw	$21,364($sp)
	sw	$20,360($sp)
	sw	$19,356($sp)
	sw	$18,352($sp)
	sw	$17,348($sp)
	sw	$16,344($sp)
	move	$fp,$6
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L448
	li	$3,-28			# 0xffffffffffffffe4
	.set	macro
	.set	reorder

	addu	$2,$5,$6
	lw	$3,24($28)
	or	$2,$5,$2
	or	$2,$2,$6
	and	$3,$3,$2
	addiu	$7,$sp,40
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L453
	move	$4,$6
	.set	macro
	.set	reorder

	move	$4,$7
#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
	move	$4,$6
$L453:
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L448
	li	$3,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addu	$2,$7,$fp
	lui	$16,%hi($LC18)
	sb	$0,0($2)
	addiu	$4,$sp,296
	addiu	$5,$16,%lo($LC18)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$7,296($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L448
	li	$3,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	lb	$4,0($2)
	li	$2,97			# 0x61
	.set	noreorder
	.set	nomacro
	beq	$4,$2,$L459
	li	$17,1			# 0x1
	.set	macro
	.set	reorder

	li	$2,100			# 0x64
	.set	noreorder
	.set	nomacro
	bne	$4,$2,$L448
	move	$17,$0
	.set	macro
	.set	reorder

$L459:
	addiu	$5,$16,%lo($LC18)
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$4,$sp,296
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L448
	li	$3,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	move	$4,$2
	lui	$16,%hi($LC17)
	addiu	$2,$sp,300
	addiu	$5,$16,%lo($LC17)
	addiu	$6,$sp,308
	addiu	$7,$sp,312
	sw	$2,16($sp)
	addiu	$2,$sp,304
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,308($sp)
	lw	$2,312($sp)
	lui	$5,%hi($LC18)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,300($sp)
	addiu	$5,$5,%lo($LC18)
	addiu	$4,$sp,296
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,304($sp)
	.set	noreorder
	.set	nomacro
	jal	strsep
	or	$23,$3,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L448
	li	$3,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	move	$4,$2
	addiu	$2,$sp,316
	addiu	$5,$16,%lo($LC17)
	sw	$2,16($sp)
	addiu	$6,$sp,324
	addiu	$2,$sp,320
	addiu	$7,$sp,328
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,324($sp)
	lw	$2,328($sp)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,316($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,320($sp)
	or	$16,$3,$2
	li	$2,536870912			# 0x20000000
	addu	$2,$16,$2
	sltu	$2,$2,256
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L607
	li	$2,-268500992			# 0xffffffffefff0000
	.set	macro
	.set	reorder

	ori	$2,$2,0xfffa
	beq	$16,$2,$L607
	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	li	$2,1			# 0x1
	bne	$17,$2,$L476
	.set	noreorder
	.set	nomacro
	jal	find_group_info
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L599
	move	$20,$2
	.set	macro
	.set	reorder

	lw	$3,4($2)
$L600:
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L608
	lui	$6,%hi(wl_mbr_head)
	.set	macro
	.set	reorder

	lw	$2,8($3)
	beq	$2,$23,$L477
	.set	noreorder
	.set	nomacro
	j	$L600
	lw	$3,56($3)
	.set	macro
	.set	reorder

$L477:
	bne	$3,$0,$L581
$L599:
	lui	$6,%hi(wl_mbr_head)
$L608:
	lw	$3,%lo(wl_mbr_head)($6)
	move	$5,$0
$L601:
	beq	$3,$0,$L484
	lw	$2,8($3)
	beq	$2,$23,$L484
	move	$5,$3
	.set	noreorder
	.set	nomacro
	j	$L601
	lw	$3,56($3)
	.set	macro
	.set	reorder

$L484:
	beq	$20,$0,$L482
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L582
	lw	$2,%lo(wl_mbr_head)($6)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L488
	lw	$4,56($3)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L489
	sw	$4,%lo(wl_mbr_head)($6)
	.set	macro
	.set	reorder

$L488:
	sw	$4,56($5)
$L489:
	sw	$0,56($3)
	lw	$2,4($20)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L490
	move	$5,$2
	.set	macro
	.set	reorder

$L602:
	lw	$2,56($5)
	beq	$2,$0,$L586
	.set	noreorder
	.set	nomacro
	j	$L602
	lw	$5,56($5)
	.set	macro
	.set	reorder

$L586:
	.set	noreorder
	.set	nomacro
	j	$L482
	sw	$3,56($5)
	.set	macro
	.set	reorder

$L490:
	sw	$3,4($20)
$L482:
	bne	$3,$0,$L581
$L582:
$L499:
	lui	$2,%hi(malloc_sizes+16)
	lw	$4,%lo(malloc_sizes+16)($2)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L581
	move	$19,$2
	.set	macro
	.set	reorder

	lui	$2,%hi(mac_cache)
	addiu	$2,$2,%lo(mac_cache)
	move	$5,$2
	move	$4,$0
	addiu	$3,$2,8
$L540:
	lw	$2,0($3)
	addiu	$4,$4,1
	slt	$6,$4,10
	.set	noreorder
	.set	nomacro
	beq	$2,$23,$L597
	addiu	$3,$3,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$6,$0,$L540
	addiu	$5,$5,16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L578
	move	$4,$19
	.set	macro
	.set	reorder

$L597:
	.set	noreorder
	.set	nomacro
	beq	$5,$0,$L541
	move	$21,$5
	.set	macro
	.set	reorder

	beq	$20,$0,$L542
	lw	$2,4($20)
	bne	$2,$0,$L542
	sw	$16,332($sp)
	lbu	$3,334($sp)
	lbu	$2,333($sp)
	lbu	$4,335($sp)
	sw	$0,340($sp)
	addu	$2,$2,$3
	addu	$2,$2,$4
	andi	$2,$2,0xf
	lui	$3,%hi(mcache)
	sll	$22,$2,2
	addiu	$2,$3,%lo(mcache)
	addu	$2,$22,$2
	lw	$18,0($2)
	sw	$16,336($sp)
$L603:
	beq	$18,$0,$L542
	lw	$2,0($18)
	beq	$2,$16,$L545
	sw	$18,340($sp)
	.set	noreorder
	.set	nomacro
	j	$L603
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L545:
	beq	$18,$0,$L542
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L609
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L551:
	lbu	$2,339($sp)
	lbu	$5,336($sp)
	lbu	$6,337($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,338($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L551
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L609:
	addiu	$2,$3,%lo(mcache)
	addu	$4,$22,$2
	lw	$2,0($4)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L552
	lw	$3,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L553
	sw	$3,0($4)
	.set	macro
	.set	reorder

$L552:
	lw	$2,340($sp)
	sw	$3,8($2)
$L553:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L542:
	lw	$2,12($21)
	addiu	$5,$21,2
	move	$4,$19
	sw	$2,52($19)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	sw	$23,8($19)
	sw	$0,56($19)
	sw	$0,16($19)
	.set	noreorder
	.set	nomacro
	beq	$20,$0,$L555
	sw	$0,12($19)
	.set	macro
	.set	reorder

	lw	$2,4($20)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L556
	move	$3,$2
	.set	macro
	.set	reorder

$L604:
	lw	$2,56($3)
	beq	$2,$0,$L595
	.set	noreorder
	.set	nomacro
	j	$L604
	lw	$3,56($3)
	.set	macro
	.set	reorder

$L556:
	.set	noreorder
	.set	nomacro
	j	$L581
	sw	$19,4($20)
	.set	macro
	.set	reorder

$L555:
	lui	$3,%hi(wl_mbr_head)
	lw	$2,%lo(wl_mbr_head)($3)
	beq	$2,$0,$L563
	move	$3,$2
$L605:
	lw	$2,56($3)
	beq	$2,$0,$L595
	.set	noreorder
	.set	nomacro
	j	$L605
	lw	$3,56($3)
	.set	macro
	.set	reorder

$L595:
	.set	noreorder
	.set	nomacro
	j	$L581
	sw	$19,56($3)
	.set	macro
	.set	reorder

$L563:
	.set	noreorder
	.set	nomacro
	j	$L581
	sw	$19,%lo(wl_mbr_head)($3)
	.set	macro
	.set	reorder

$L541:
	.set	noreorder
	.set	nomacro
	j	$L578
	move	$4,$19
	.set	macro
	.set	reorder

$L476:
	.set	noreorder
	.set	nomacro
	jal	find_group_info
	move	$4,$16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L581
	move	$3,$2
	.set	macro
	.set	reorder

	lw	$4,4($2)
	move	$6,$0
	move	$5,$4
$L606:
	beq	$4,$0,$L581
	lw	$2,8($4)
	beq	$2,$23,$L573
	move	$6,$4
	.set	noreorder
	.set	nomacro
	j	$L606
	lw	$4,56($4)
	.set	macro
	.set	reorder

$L573:
	beq	$4,$0,$L581
	.set	noreorder
	.set	nomacro
	bne	$4,$5,$L577
	lw	$2,56($4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L578
	sw	$2,4($3)
	.set	macro
	.set	reorder

$L577:
	sw	$2,56($6)
$L578:
	jal	kfree
$L581:
	jal	local_bh_enable
$L607:
	move	$3,$fp
$L448:
	lw	$31,380($sp)
	lw	$fp,376($sp)
	lw	$23,372($sp)
	lw	$22,368($sp)
	lw	$21,364($sp)
	lw	$20,360($sp)
	lw	$19,356($sp)
	lw	$18,352($sp)
	lw	$17,348($sp)
	lw	$16,344($sp)
	move	$2,$3
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,384
	.set	macro
	.set	reorder

	.end	wl_mcast_write
	.section	.rodata.str1.4
	.align	2
$LC27:
	.ascii	"without wirless client\n\000"
	.align	2
$LC28:
	.ascii	"wireless client %u.%u.%u.%u mac %02x:%02x:%02x:%02x:%02x"
	.ascii	":%02x dev %s\n\000"
	.align	2
$LC29:
	.ascii	"mode %s, list \000"
	.align	2
$LC30:
	.ascii	"mode %s, list empty\n\000"
	.text
	.align	2
	.ent	wl_mcast_set_read
	.type	wl_mcast_set_read, @function
wl_mcast_set_read:
	.frame	$sp,96,$31		# vars= 0, regs= 10/0, args= 56, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-96
	sw	$fp,88($sp)
	sw	$23,84($sp)
	sw	$22,80($sp)
	sw	$21,76($sp)
	sw	$18,64($sp)
	sw	$31,92($sp)
	sw	$20,72($sp)
	sw	$19,68($sp)
	sw	$17,60($sp)
	sw	$16,56($sp)
	move	$21,$4
	sw	$5,100($sp)
	move	$fp,$6
	move	$23,$7
	move	$18,$0
	move	$22,$0
	lui	$2,%hi(mgroup_info)
$L650:
	sll	$3,$22,2
	addiu	$2,$2,%lo(mgroup_info)
	addu	$3,$3,$2
	lw	$19,0($3)
$L647:
	beq	$19,$0,$L642
	lui	$5,%hi($LC3)

	lbu	$2,2($19)
	lbu	$6,0($19)
	lbu	$7,1($19)
	sw	$2,16($sp)
	lbu	$2,3($19)
	addu	$4,$21,$18
	addiu	$5,$5,%lo($LC3)
	jal	sprintf
	sw	$2,20($sp)

	lw	$16,4($19)
	bne	$16,$0,$L633
	addu	$18,$18,$2

	lui	$5,%hi($LC27)
	addu	$4,$21,$18
	jal	sprintf
	addiu	$5,$5,%lo($LC27)

	j	$L644
	addu	$18,$18,$2

$L633:
	lbu	$2,10($16)
	lbu	$6,8($16)
	lbu	$7,9($16)
	sw	$2,16($sp)
	lbu	$2,11($16)
	lui	$5,%hi($LC28)
	addu	$4,$21,$18
	sw	$2,20($sp)
	lbu	$2,0($16)
	addiu	$5,$5,%lo($LC28)
	sw	$2,24($sp)
	lbu	$2,1($16)
	sw	$2,28($sp)
	lbu	$2,2($16)
	sw	$2,32($sp)
	lbu	$2,3($16)
	sw	$2,36($sp)
	lbu	$2,4($16)
	sw	$2,40($sp)
	lbu	$2,5($16)
	sw	$2,44($sp)
	lw	$2,52($16)
	jal	sprintf
	sw	$2,48($sp)

	lw	$3,12($16)
	addu	$18,$18,$2
	addu	$4,$21,$18
	beq	$3,$0,$L619
	lw	$5,16($16)

	li	$2,1			# 0x1
	bne	$5,$2,$L622
	lui	$2,%hi($LC6)

	lui	$2,%hi($LC5)
	j	$L623
	addiu	$6,$2,%lo($LC5)

$L622:
	addiu	$6,$2,%lo($LC6)
$L623:
	lui	$5,%hi($LC29)
	jal	sprintf
	addiu	$5,$5,%lo($LC29)

	lw	$3,12($16)
	addu	$18,$18,$2
	blez	$3,$L646
	move	$20,$0

	move	$17,$16
$L627:
	lbu	$2,22($17)
	lbu	$6,20($17)
	lbu	$7,21($17)
	sw	$2,16($sp)
	lbu	$2,23($17)
	lui	$5,%hi($LC7)
	addu	$4,$21,$18
	addiu	$5,$5,%lo($LC7)
	jal	sprintf
	sw	$2,20($sp)

	lw	$3,12($16)
	addiu	$20,$20,1
	addu	$18,$18,$2
	slt	$3,$20,$3
	bne	$3,$0,$L627
	addiu	$17,$17,4

$L646:
	lui	$5,%hi($LC8)
	addiu	$5,$5,%lo($LC8)
	jal	sprintf
	addu	$4,$21,$18

	j	$L649
	lw	$16,56($16)

$L619:
	li	$2,1			# 0x1
	bne	$5,$2,$L631
	lui	$2,%hi($LC6)

	lui	$2,%hi($LC5)
	j	$L632
	addiu	$6,$2,%lo($LC5)

$L631:
	addiu	$6,$2,%lo($LC6)
$L632:
	lui	$5,%hi($LC30)
	jal	sprintf
	addiu	$5,$5,%lo($LC30)

	lw	$16,56($16)
$L649:
	bne	$16,$0,$L633
	addu	$18,$18,$2

$L644:
	j	$L647
	lw	$19,128($19)

$L642:
	addiu	$22,$22,1
	slt	$2,$22,16
	bne	$2,$0,$L650
	lui	$2,%hi(mgroup_info)

	addu	$2,$fp,$23
	slt	$2,$2,$18
	bne	$2,$0,$L636
	lw	$2,112($sp)

	li	$3,1			# 0x1
	sw	$3,0($2)
$L636:
	subu	$18,$18,$fp
	lw	$4,100($sp)
	slt	$2,$23,$18
	movn	$18,$23,$2
	addu	$2,$21,$fp
	slt	$3,$18,0
	sw	$2,0($4)
	move	$2,$0
	movz	$2,$18,$3
	lw	$31,92($sp)
	lw	$fp,88($sp)
	lw	$23,84($sp)
	lw	$22,80($sp)
	lw	$21,76($sp)
	lw	$20,72($sp)
	lw	$19,68($sp)
	lw	$18,64($sp)
	lw	$17,60($sp)
	lw	$16,56($sp)
	j	$31
	addiu	$sp,$sp,96

	.set	macro
	.set	reorder
	.end	wl_mcast_set_read
	.align	2
	.ent	wl_mcast_set_write
	.type	wl_mcast_set_write, @function
wl_mcast_set_write:
	.frame	$sp,416,$31		# vars= 352, regs= 9/0, args= 24, gp= 0
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	addiu	$sp,$sp,-416
	sltu	$2,$6,257
	sw	$19,388($sp)
	sw	$31,408($sp)
	sw	$23,404($sp)
	sw	$22,400($sp)
	sw	$21,396($sp)
	sw	$20,392($sp)
	sw	$18,384($sp)
	sw	$17,380($sp)
	sw	$16,376($sp)
	move	$19,$6
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L651
	li	$3,-28			# 0xffffffffffffffe4
	.set	macro
	.set	reorder

	addu	$2,$5,$6
	lw	$3,24($28)
	or	$2,$5,$2
	or	$2,$2,$6
	and	$3,$3,$2
	addiu	$7,$sp,24
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L656
	move	$4,$6
	.set	macro
	.set	reorder

	move	$4,$7
#APP
	.set	noreorder
	jal	__copy_user
	.set	noat
	addu	$1, $5, $6
	.set	at
	.set	reorder
#NO_APP
	move	$4,$6
$L656:
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L651
	li	$3,-14			# 0xfffffffffffffff2
	.set	macro
	.set	reorder

	addu	$2,$7,$19
	lui	$20,%hi($LC18)
	sb	$0,0($2)
	addiu	$4,$sp,320
	addiu	$5,$20,%lo($LC18)
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$7,320($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L651
	li	$3,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	move	$4,$2
	lui	$22,%hi($LC17)
	addiu	$2,$sp,324
	addiu	$5,$22,%lo($LC17)
	addiu	$6,$sp,332
	addiu	$7,$sp,336
	sw	$2,16($sp)
	addiu	$2,$sp,328
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,332($sp)
	lw	$2,336($sp)
	addiu	$4,$sp,320
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,324($sp)
	addiu	$5,$20,%lo($LC18)
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,328($sp)
	.set	noreorder
	.set	nomacro
	jal	strsep
	or	$21,$3,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L651
	li	$3,-22			# 0xffffffffffffffea
	.set	macro
	.set	reorder

	move	$4,$2
	addiu	$2,$sp,340
	addiu	$7,$sp,352
	addiu	$5,$22,%lo($LC17)
	addiu	$6,$sp,348
	sw	$2,16($sp)
	addiu	$2,$sp,344
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,348($sp)
	lw	$2,352($sp)
	addiu	$23,$sp,280
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,340($sp)
	move	$4,$23
	move	$5,$0
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,344($sp)
	li	$6,40			# 0x28
	.set	noreorder
	.set	nomacro
	jal	memset
	or	$18,$3,$2
	.set	macro
	.set	reorder

	addiu	$4,$sp,320
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$20,%lo($LC18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L651
	move	$3,$19
	.set	macro
	.set	reorder

	lb	$2,0($2)
	addiu	$4,$sp,320
	addiu	$5,$20,%lo($LC18)
	addiu	$2,$2,-48
	.set	noreorder
	.set	nomacro
	jal	strsep
	sw	$2,284($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L651
	move	$3,$19
	.set	macro
	.set	reorder

	lb	$2,0($2)
	li	$3,-22			# 0xffffffffffffffea
	addiu	$4,$2,-48
	slt	$2,$4,9
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L651
	sw	$4,280($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$4,$L718
	move	$17,$0
	.set	macro
	.set	reorder

	addiu	$16,$sp,288
$L672:
	addiu	$4,$sp,320
	.set	noreorder
	.set	nomacro
	jal	strsep
	addiu	$5,$20,%lo($LC18)
	.set	macro
	.set	reorder

	addiu	$17,$17,1
	move	$4,$2
	addiu	$5,$22,%lo($LC17)
	addiu	$6,$sp,364
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L731
	addiu	$7,$sp,368
	.set	macro
	.set	reorder

	addiu	$2,$sp,356
	sw	$2,16($sp)
	addiu	$2,$sp,360
	.set	noreorder
	.set	nomacro
	jal	sscanf
	sw	$2,20($sp)
	.set	macro
	.set	reorder

	lw	$3,364($sp)
	lw	$2,368($sp)
	sll	$3,$3,24
	sll	$2,$2,16
	or	$3,$3,$2
	lw	$2,356($sp)
	sll	$2,$2,8
	or	$3,$3,$2
	lw	$2,360($sp)
	or	$3,$3,$2
	lw	$2,280($sp)
	sw	$3,0($16)
	slt	$2,$17,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L672
	addiu	$16,$16,4
	.set	macro
	.set	reorder

$L718:
	li	$2,536870912			# 0x20000000
	addu	$2,$18,$2
	sltu	$2,$2,256
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L651
	move	$3,$19
	.set	macro
	.set	reorder

	li	$2,-268500992			# 0xffffffffefff0000
	ori	$2,$2,0xfffa
	.set	noreorder
	.set	nomacro
	beq	$18,$2,$L732
	lw	$31,408($sp)
	.set	macro
	.set	reorder

	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	.set	noreorder
	.set	nomacro
	jal	find_group_info
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L726
	move	$6,$2
	.set	macro
	.set	reorder

	lw	$4,4($2)
$L727:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L733
	lui	$7,%hi(wl_mbr_head)
	.set	macro
	.set	reorder

	lw	$2,8($4)
	beq	$2,$21,$L683
	.set	noreorder
	.set	nomacro
	j	$L727
	lw	$4,56($4)
	.set	macro
	.set	reorder

$L683:
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L734
	lw	$2,280($sp)
	.set	macro
	.set	reorder

$L726:
	lui	$7,%hi(wl_mbr_head)
$L733:
	lw	$4,%lo(wl_mbr_head)($7)
	move	$5,$0
$L728:
	beq	$4,$0,$L690
	lw	$2,8($4)
	beq	$2,$21,$L690
	move	$5,$4
	.set	noreorder
	.set	nomacro
	j	$L728
	lw	$4,56($4)
	.set	macro
	.set	reorder

$L690:
	beq	$6,$0,$L688
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L715
	lw	$2,%lo(wl_mbr_head)($7)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$4,$L694
	lw	$3,56($4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L695
	sw	$3,%lo(wl_mbr_head)($7)
	.set	macro
	.set	reorder

$L694:
	sw	$3,56($5)
$L695:
	sw	$0,56($4)
	lw	$2,4($6)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L696
	move	$5,$2
	.set	macro
	.set	reorder

$L729:
	lw	$2,56($5)
	beq	$2,$0,$L722
	.set	noreorder
	.set	nomacro
	j	$L729
	lw	$5,56($5)
	.set	macro
	.set	reorder

$L722:
	.set	noreorder
	.set	nomacro
	j	$L688
	sw	$4,56($5)
	.set	macro
	.set	reorder

$L696:
	sw	$4,4($6)
$L688:
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L715
	lw	$2,280($sp)
	.set	macro
	.set	reorder

$L734:
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L703
	lw	$3,284($sp)
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L735
	addiu	$4,$4,12
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	find_group_info
	move	$4,$18
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L715
	move	$3,$2
	.set	macro
	.set	reorder

	lw	$4,4($2)
	move	$6,$0
	move	$5,$4
$L730:
	beq	$4,$0,$L715
	lw	$2,8($4)
	beq	$2,$21,$L706
	move	$6,$4
	.set	noreorder
	.set	nomacro
	j	$L730
	lw	$4,56($4)
	.set	macro
	.set	reorder

$L706:
	beq	$4,$0,$L715
	.set	noreorder
	.set	nomacro
	bne	$4,$5,$L710
	lw	$2,56($4)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L711
	sw	$2,4($3)
	.set	macro
	.set	reorder

$L710:
	sw	$2,56($6)
$L711:
	jal	kfree
	j	$L715
$L703:
	addiu	$4,$4,12
$L735:
	move	$5,$23
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

$L715:
	jal	local_bh_enable
$L731:
	move	$3,$19
$L651:
	lw	$31,408($sp)
$L732:
	lw	$23,404($sp)
	lw	$22,400($sp)
	lw	$21,396($sp)
	lw	$20,392($sp)
	lw	$19,388($sp)
	lw	$18,384($sp)
	lw	$17,380($sp)
	lw	$16,376($sp)
	move	$2,$3
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,416
	.set	macro
	.set	reorder

	.end	wl_mcast_set_write
	.section	.rodata.str1.4
	.align	2
$LC31:
	.ascii	"br_igmpsnoop\000"
	.align	2
$LC32:
	.ascii	"mcast_entry\000"
	.align	2
$LC33:
	.ascii	"wl_mcast\000"
	.align	2
$LC34:
	.ascii	"wl_mcast_set\000"
	.align	2
$LC35:
	.ascii	"iptv_mcast_entry\000"
	.text
	.align	2
	.globl	br_igmpv3_init
	.ent	br_igmpv3_init
	.type	br_igmpv3_init, @function
br_igmpv3_init:
	.frame	$sp,32,$31		# vars= 8, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$4,%hi($LC31)
	addiu	$sp,$sp,-32
	addiu	$4,$4,%lo($LC31)
	move	$5,$0
	sw	$31,24($sp)
	jal	create_proc_entry
	move	$6,$0

	lui	$4,%hi(procigmp)
	move	$3,$2
	beq	$2,$0,$L738
	sw	$2,%lo(procigmp)($4)

	lui	$2,%hi(br_igmpread_proc)
	addiu	$2,$2,%lo(br_igmpread_proc)
	sw	$2,64($3)
	lw	$3,%lo(procigmp)($4)
	lui	$2,%hi(br_igmpwrite_proc)
	addiu	$2,$2,%lo(br_igmpwrite_proc)
	sw	$2,68($3)
$L738:
	lui	$4,%hi($LC32)
	addiu	$4,$4,%lo($LC32)
	move	$5,$0
	jal	create_proc_entry
	move	$6,$0

	lui	$4,%hi(mcast_set_entry)
	move	$3,$2
	beq	$2,$0,$L739
	sw	$2,%lo(mcast_set_entry)($4)

	lui	$2,%hi(br_mcast_entry_read)
	addiu	$2,$2,%lo(br_mcast_entry_read)
	sw	$2,64($3)
	lw	$3,%lo(mcast_set_entry)($4)
	lui	$2,%hi(br_mcast_entry_write)
	addiu	$2,$2,%lo(br_mcast_entry_write)
	sw	$2,68($3)
$L739:
	lui	$4,%hi($LC33)
	addiu	$4,$4,%lo($LC33)
	move	$5,$0
	jal	create_proc_entry
	move	$6,$0

	lui	$4,%hi(wl_mcast_entry)
	move	$3,$2
	beq	$2,$0,$L740
	sw	$2,%lo(wl_mcast_entry)($4)

	lui	$2,%hi(wl_mcast_read)
	addiu	$2,$2,%lo(wl_mcast_read)
	sw	$2,64($3)
	lw	$3,%lo(wl_mcast_entry)($4)
	lui	$2,%hi(wl_mcast_write)
	addiu	$2,$2,%lo(wl_mcast_write)
	sw	$2,68($3)
$L740:
	lui	$4,%hi($LC34)
	addiu	$4,$4,%lo($LC34)
	move	$5,$0
	jal	create_proc_entry
	move	$6,$0

	lui	$4,%hi(wl_mcast_set_entry)
	move	$3,$2
	beq	$2,$0,$L741
	sw	$2,%lo(wl_mcast_set_entry)($4)

	lui	$2,%hi(wl_mcast_set_read)
	addiu	$2,$2,%lo(wl_mcast_set_read)
	sw	$2,64($3)
	lw	$3,%lo(wl_mcast_set_entry)($4)
	lui	$2,%hi(wl_mcast_set_write)
	addiu	$2,$2,%lo(wl_mcast_set_write)
	sw	$2,68($3)
$L741:
	lui	$4,%hi($LC35)
	addiu	$4,$4,%lo($LC35)
	move	$5,$0
	jal	create_proc_entry
	move	$6,$0

	lui	$4,%hi(iptv_mcast_set_entry)
	move	$3,$2
	beq	$2,$0,$L742
	sw	$2,%lo(iptv_mcast_set_entry)($4)

	lui	$2,%hi(br_iptv_mcast_entry_read)
	addiu	$2,$2,%lo(br_iptv_mcast_entry_read)
	sw	$2,64($3)
	lw	$3,%lo(iptv_mcast_set_entry)($4)
	lui	$2,%hi(br_iptv_mcast_entry_write)
	addiu	$2,$2,%lo(br_iptv_mcast_entry_write)
	sw	$2,68($3)
$L742:
	lui	$4,%hi(mcache)
	addiu	$4,$4,%lo(mcache)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	lui	$4,%hi(mgroup_info)
	addiu	$4,$4,%lo(mgroup_info)
	move	$5,$0
	jal	memset
	li	$6,64			# 0x40

	lw	$31,24($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
	.end	br_igmpv3_init
	.align	2
	.globl	iptv_wireless_multicast_filter
	.ent	iptv_wireless_multicast_filter
	.type	iptv_wireless_multicast_filter, @function
iptv_wireless_multicast_filter:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	sll	$11,$7,3
	subu	$2,$11,$7
	sll	$2,$2,3
	addu	$8,$2,$5
	lw	$3,20($8)
	bne	$3,$0,$L744
	li	$2,1			# 0x1

	move	$2,$3
	beq	$2,$0,$L746
	lw	$3,16($8)

	blez	$3,$L792
	move	$7,$0

	move	$9,$3
	addiu	$3,$8,24
$L752:
	lw	$2,0($3)
	addiu	$7,$7,1
	slt	$5,$7,$9
	beq	$4,$2,$L786
	addiu	$3,$3,4

	bne	$5,$0,$L752
	nop

$L792:
	j	$L753
	move	$2,$0

$L746:
	blez	$3,$L786
	move	$7,$0

	move	$9,$3
	addiu	$3,$8,24
$L760:
	lw	$2,0($3)
	addiu	$7,$7,1
	slt	$5,$7,$9
	beq	$4,$2,$L792
	addiu	$3,$3,4

	bne	$5,$0,$L760
	nop

$L786:
	li	$2,1			# 0x1
$L753:
	bne	$2,$0,$L797
	li	$2,-128

	j	$31
	sb	$0,0($6)

$L744:
	bne	$3,$2,$L796
	nop

	lw	$2,20($8)
	beq	$2,$0,$L765
	lw	$3,16($8)

	blez	$3,$L794
	move	$9,$0

	move	$10,$3
	addiu	$3,$8,24
$L771:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$8,$9,$10
	beq	$4,$2,$L790
	addiu	$3,$3,4

	bne	$8,$0,$L771
	nop

$L794:
	j	$L772
	move	$2,$0

$L765:
	blez	$3,$L790
	move	$9,$0

	move	$10,$3
	addiu	$3,$8,24
$L779:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$8,$9,$10
	beq	$4,$2,$L794
	addiu	$3,$3,4

	bne	$8,$0,$L779
	nop

$L790:
	li	$2,1			# 0x1
$L772:
	bne	$2,$0,$L764
	li	$2,-128

	subu	$2,$11,$7
	sll	$2,$2,3
	addu	$2,$2,$5
	lw	$2,16($2)
	blez	$2,$L780
	nop

	j	$31
	sb	$0,0($6)

$L780:
	li	$2,-128
$L797:
	j	$31
	sb	$2,0($6)

$L764:
	sb	$2,0($6)
$L796:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	iptv_wireless_multicast_filter
	.align	2
	.globl	iptv_wire_multicast_deny_filter
	.ent	iptv_wire_multicast_deny_filter
	.type	iptv_wire_multicast_deny_filter, @function
iptv_wire_multicast_deny_filter:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	sll	$2,$5,3
	subu	$2,$2,$5
	sll	$2,$2,3
	addu	$2,$2,$4
	lbu	$8,11($2)
	move	$3,$0
	sra	$2,$8,$3
$L876:
	andi	$2,$2,0x1
	movn	$11,$3,$2
	addiu	$3,$3,1
	slt	$2,$3,2
	bne	$2,$0,$L876
	sra	$2,$8,$3

	move	$12,$0
$L854:
	beq	$5,$12,$L806
	nop

	lbu	$2,10($4)
	beq	$2,$0,$L806
	nop

	lbu	$3,11($4)
	andi	$2,$3,0x7f
	beq	$2,$0,$L806
	nop

	move	$8,$3
	li	$9,1			# 0x1
	move	$3,$0
	sra	$2,$8,$3
$L878:
	andi	$2,$2,0x1
	beq	$2,$0,$L810
	nop

	bne	$3,$11,$L806
	nop

	lw	$2,20($4)
	bne	$2,$9,$L813
	nop

	lw	$3,16($4)
	beq	$3,$0,$L879
	nop

	beq	$2,$0,$L816
	slt	$8,$0,$3

	beq	$8,$0,$L872
	move	$9,$0

	move	$10,$3
	addiu	$3,$4,24
$L822:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$8,$9,$10
	beq	$7,$2,$L866
	addiu	$3,$3,4

	bne	$8,$0,$L822
	nop

$L872:
	j	$L823
	move	$2,$0

$L816:
	beq	$8,$0,$L866
	move	$9,$0

	move	$10,$3
	addiu	$3,$4,24
$L830:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$8,$9,$10
	beq	$7,$2,$L872
	addiu	$3,$3,4

	bne	$8,$0,$L830
	nop

$L866:
	li	$2,1			# 0x1
$L823:
	bne	$2,$0,$L879
	addiu	$12,$12,1

	j	$L880
	slt	$2,$12,8

$L813:
	bne	$2,$0,$L877
	addiu	$3,$3,1

	lw	$2,20($4)
	beq	$2,$0,$L835
	lw	$3,16($4)

	blez	$3,$L874
	move	$9,$0

	move	$10,$3
	addiu	$3,$4,24
$L841:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$8,$9,$10
	beq	$7,$2,$L870
	addiu	$3,$3,4

	bne	$8,$0,$L841
	nop

$L874:
	j	$L842
	move	$2,$0

$L835:
	blez	$3,$L870
	move	$9,$0

	move	$10,$3
	addiu	$3,$4,24
$L849:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$8,$9,$10
	beq	$7,$2,$L874
	addiu	$3,$3,4

	bne	$8,$0,$L849
	nop

$L870:
	li	$2,1			# 0x1
$L842:
	beq	$2,$0,$L875
	addiu	$12,$12,1

	j	$31
	addiu	$12,$12,-1

$L810:
	addiu	$3,$3,1
$L877:
	slt	$2,$3,2
	bne	$2,$0,$L878
	sra	$2,$8,$3

$L806:
	addiu	$12,$12,1
$L875:
	slt	$2,$12,8
$L880:
	bne	$2,$0,$L854
	addiu	$4,$4,56

	bne	$11,$0,$L855
	li	$2,1			# 0x1

	lw	$2,8($6)
	andi	$2,$2,0xfe
	j	$31
	sw	$2,8($6)

$L855:
	bne	$11,$2,$L879
	nop

	lw	$2,8($6)
	andi	$2,$2,0xfd
	sw	$2,8($6)
$L879:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	iptv_wire_multicast_deny_filter
	.align	2
	.globl	iptv_wire_multicast_filter
	.ent	iptv_wire_multicast_filter
	.type	iptv_wire_multicast_filter, @function
iptv_wire_multicast_filter:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	sll	$10,$6,3
	subu	$2,$10,$6
	sll	$2,$2,3
	addu	$8,$2,$5
	lbu	$2,10($8)
	beq	$2,$0,$L882
	move	$11,$4

	lbu	$2,11($8)
	andi	$2,$2,0x7f
	beq	$2,$0,$L935
	subu	$2,$10,$6

	lw	$2,20($8)
	bne	$2,$0,$L882
	nop

	beq	$2,$0,$L884
	lw	$3,16($8)

	blez	$3,$L930
	move	$9,$0

	move	$10,$3
	addiu	$3,$8,24
$L890:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$4,$9,$10
	beq	$11,$2,$L924
	addiu	$3,$3,4

	bne	$4,$0,$L890
	nop

$L930:
	j	$L891
	move	$2,$0

$L884:
	blez	$3,$L924
	move	$9,$0

	move	$10,$3
	addiu	$3,$8,24
$L898:
	lw	$2,0($3)
	addiu	$9,$9,1
	slt	$4,$9,$10
	beq	$11,$2,$L930
	addiu	$3,$3,4

	bne	$4,$0,$L898
	nop

$L924:
	li	$2,1			# 0x1
$L891:
	bne	$2,$0,$L936
	move	$4,$5

	j	$L937
	move	$5,$6

$L882:
	subu	$2,$10,$6
$L935:
	sll	$2,$2,3
	addu	$4,$2,$5
	lbu	$2,10($4)
	beq	$2,$0,$L936
	nop

	lbu	$2,11($4)
	andi	$2,$2,0x7f
	beq	$2,$0,$L936
	li	$2,1			# 0x1

	lw	$3,20($4)
	bne	$3,$2,$L936
	move	$2,$3

	beq	$2,$0,$L903
	lw	$3,16($4)

	blez	$3,$L932
	move	$8,$0

	move	$9,$3
	addiu	$3,$4,24
$L909:
	lw	$2,0($3)
	addiu	$8,$8,1
	slt	$4,$8,$9
	beq	$11,$2,$L928
	addiu	$3,$3,4

	bne	$4,$0,$L909
	nop

$L932:
	j	$L910
	move	$2,$0

$L903:
	blez	$3,$L928
	move	$8,$0

	move	$9,$3
	addiu	$3,$4,24
$L917:
	lw	$2,0($3)
	addiu	$8,$8,1
	slt	$4,$8,$9
	beq	$11,$2,$L932
	addiu	$3,$3,4

	bne	$4,$0,$L917
	nop

$L928:
	li	$2,1			# 0x1
$L910:
	bne	$2,$0,$L936
	subu	$2,$10,$6

	sll	$2,$2,3
	addu	$2,$2,$5
	lw	$2,16($2)
	beq	$2,$0,$L936
	move	$4,$5

	move	$5,$6
$L937:
	move	$6,$7
	j	iptv_wire_multicast_deny_filter
	move	$7,$11

$L936:
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	iptv_wire_multicast_filter
	.section	.rodata.str1.4
	.align	2
$LC36:
	.ascii	"add_server_cache: cannot find group info!\n\000"
	.align	2
$LC37:
	.ascii	"add_server_cache: find the server in cache!\n\000"
	.align	2
$LC38:
	.ascii	"1. Add group [%d.%d.%d.%d] server cache[%d.%d.%d.%d]\n\000"
	.align	2
$LC39:
	.ascii	"2. Add group [%d.%d.%d.%d] server cache[%d.%d.%d.%d]\n\000"
	.text
	.align	2
	.globl	add_server_cache
	.ent	add_server_cache
	.type	add_server_cache, @function
add_server_cache:
	.frame	$sp,80,$31		# vars= 8, regs= 8/0, args= 40, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$4,40($sp)
	lbu	$2,41($sp)
	lbu	$3,42($sp)
	lbu	$6,43($sp)
	sw	$22,72($sp)
	addu	$2,$2,$3
	addu	$2,$2,$6
	andi	$2,$2,0xf
	lui	$22,%hi(mcache)
	sw	$21,68($sp)
	sll	$21,$2,2
	addiu	$2,$22,%lo(mcache)
	sw	$20,64($sp)
	sw	$31,76($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	addu	$2,$21,$2
	sw	$5,84($sp)
	sw	$4,80($sp)
	jal	find_group_info
	lw	$17,0($2)

	bne	$2,$0,$L940
	move	$20,$2

	lui	$4,%hi($LC36)
	jal	printk
	addiu	$4,$4,%lo($LC36)

	j	$L938
	move	$4,$0

$L940:
	beq	$17,$0,$L945
	lw	$3,80($sp)

$L944:
	lw	$2,0($17)
	beq	$2,$3,$L942
	nop

	lw	$17,8($17)
	bne	$17,$0,$L944
	lui	$16,%hi(malloc_sizes)

	j	$L1158
	addiu	$16,$16,%lo(malloc_sizes)

$L942:
	beq	$17,$0,$L1157
	lui	$16,%hi(malloc_sizes)

	lw	$16,4($17)
	beq	$16,$0,$L1152
	lw	$3,84($sp)

$L949:
	lw	$2,0($16)
	beq	$2,$3,$L1147
	lui	$4,%hi($LC37)

	lw	$16,12($16)
	bne	$16,$0,$L949
	nop

$L953:
$L1152:
	lui	$2,%hi(malloc_sizes+4)
	lw	$4,%lo(malloc_sizes+4)($2)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	beq	$2,$0,$L1018
	move	$16,$2

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,16			# 0x10

	lw	$3,8($20)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L989
	lw	$9,84($sp)

	lw	$2,8($16)
	ori	$2,$2,0x80
	sw	$2,8($16)
$L989:
	addiu	$6,$20,40
	li	$8,1			# 0x1
	li	$11,-1			# 0xffffffffffffffff
	li	$10,1			# 0x1
$L1010:
	lw	$3,8($6)
	beq	$3,$11,$L992
	nop

	lw	$2,12($6)
	beq	$2,$0,$L995
	slt	$4,$0,$3

	beq	$4,$0,$L1149
	move	$5,$0

	move	$7,$3
	addiu	$3,$6,16
$L1001:
	lw	$2,0($3)
	addiu	$5,$5,1
	slt	$4,$5,$7
	beq	$9,$2,$L1136
	addiu	$3,$3,4

	bne	$4,$0,$L1001
	nop

$L1149:
	j	$L1002
	move	$2,$0

$L995:
	beq	$4,$0,$L1136
	move	$5,$0

	move	$7,$3
	addiu	$3,$6,16
$L1009:
	lw	$2,0($3)
	addiu	$5,$5,1
	slt	$4,$5,$7
	beq	$9,$2,$L1149
	addiu	$3,$3,4

	bne	$4,$0,$L1009
	nop

$L1136:
	li	$2,1			# 0x1
$L1002:
	beq	$2,$0,$L992
	addiu	$2,$8,-1

	lw	$3,8($16)
	sll	$2,$10,$2
	or	$3,$3,$2
	sw	$3,8($16)
$L992:
	addiu	$8,$8,1
	slt	$2,$8,3
	bne	$2,$0,$L1010
	addiu	$6,$6,40

	lw	$2,8($16)
	beq	$2,$0,$L1156
	move	$4,$16

	lw	$2,84($sp)
	li	$3,129			# 0x81
	sw	$3,4($16)
	sw	$2,0($16)
	lw	$3,4($17)
$L1153:
	lw	$2,12($3)
	beq	$2,$0,$L1138
	nop

	j	$L1153
	lw	$3,12($3)

$L1138:
	sw	$16,12($3)
	lbu	$2,83($sp)
	lbu	$3,84($sp)
	lbu	$8,85($sp)
	lbu	$9,86($sp)
	lbu	$10,87($sp)
	lbu	$5,80($sp)
	lbu	$6,81($sp)
	lbu	$7,82($sp)
	lui	$4,%hi($LC38)
	addiu	$4,$4,%lo($LC38)
	sw	$2,16($sp)
	sw	$3,20($sp)
	sw	$8,24($sp)
	sw	$9,28($sp)
	jal	printk
	sw	$10,32($sp)

	j	$L938
	move	$4,$16

$L945:
$L1021:
$L1058:
	lui	$16,%hi(malloc_sizes)
$L1157:
	addiu	$16,$16,%lo(malloc_sizes)
$L1158:
	lw	$4,4($16)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lw	$4,4($16)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	move	$19,$2

	move	$16,$2
	beq	$19,$0,$L1018
	move	$18,$2

	beq	$2,$0,$L1094
	move	$4,$19

	move	$5,$0
	jal	memset
	li	$6,12			# 0xc

	move	$4,$16
	move	$5,$0
	jal	memset
	li	$6,16			# 0x10

	lw	$2,80($sp)
	sw	$2,0($19)
	lw	$3,8($20)
	li	$2,-1			# 0xffffffffffffffff
	beq	$3,$2,$L1095
	lw	$9,84($sp)

	lw	$2,8($16)
	ori	$2,$2,0x80
	sw	$2,8($16)
$L1095:
	addiu	$6,$20,40
	li	$8,1			# 0x1
	li	$11,-1			# 0xffffffffffffffff
	li	$10,1			# 0x1
$L1116:
	lw	$3,8($6)
	beq	$3,$11,$L1098
	nop

	lw	$2,12($6)
	beq	$2,$0,$L1101
	slt	$4,$0,$3

	beq	$4,$0,$L1151
	move	$5,$0

	move	$7,$3
	addiu	$3,$6,16
$L1107:
	lw	$2,0($3)
	addiu	$5,$5,1
	slt	$4,$5,$7
	beq	$9,$2,$L1144
	addiu	$3,$3,4

	bne	$4,$0,$L1107
	nop

$L1151:
	j	$L1108
	move	$2,$0

$L1101:
	beq	$4,$0,$L1144
	move	$5,$0

	move	$7,$3
	addiu	$3,$6,16
$L1115:
	lw	$2,0($3)
	addiu	$5,$5,1
	slt	$4,$5,$7
	beq	$9,$2,$L1151
	addiu	$3,$3,4

	bne	$4,$0,$L1115
	nop

$L1144:
	li	$2,1			# 0x1
$L1108:
	beq	$2,$0,$L1098
	addiu	$2,$8,-1

	lw	$3,8($18)
	sll	$2,$10,$2
	or	$3,$3,$2
	sw	$3,8($18)
$L1098:
	addiu	$8,$8,1
	slt	$2,$8,3
	bne	$2,$0,$L1116
	addiu	$6,$6,40

	lw	$2,8($18)
	beq	$2,$0,$L1119
	addiu	$2,$22,%lo(mcache)

	addu	$3,$21,$2
	lw	$2,0($3)
	beq	$2,$0,$L1120
	move	$17,$2

$L1154:
	lw	$2,8($17)
	beq	$2,$0,$L1146
	nop

	j	$L1154
	lw	$17,8($17)

$L1146:
	j	$L1126
	sw	$19,8($17)

$L1120:
	j	$L1126
	sw	$19,0($3)

$L1119:
	jal	kfree
	move	$4,$18

	j	$L1156
	move	$4,$19

$L1126:
	lw	$2,84($sp)
	lui	$4,%hi($LC39)
	addiu	$4,$4,%lo($LC39)
	sw	$2,0($18)
	lbu	$2,83($sp)
	lbu	$3,84($sp)
	lbu	$8,85($sp)
	lbu	$9,86($sp)
	lbu	$10,87($sp)
	lbu	$5,80($sp)
	lbu	$6,81($sp)
	lbu	$7,82($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	sw	$8,24($sp)
	sw	$9,28($sp)
	jal	printk
	sw	$10,32($sp)

	li	$2,129			# 0x81
	move	$4,$18
	sw	$2,4($18)
	j	$L938
	sw	$18,4($19)

$L1147:
	jal	printk
	addiu	$4,$4,%lo($LC37)

	j	$L938
	move	$4,$16

$L1094:
$L1156:
	jal	kfree
	nop

$L1018:
	move	$4,$0
$L938:
	lw	$31,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	move	$2,$4
	j	$31
	addiu	$sp,$sp,80

	.set	macro
	.set	reorder
	.end	add_server_cache
	.align	2
	.globl	find_server_cache
	.ent	find_server_cache
	.type	find_server_cache, @function
find_server_cache:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$4,0($sp)
	lbu	$2,2($sp)
	lbu	$3,1($sp)
	move	$6,$4
	lbu	$4,3($sp)
	addu	$3,$3,$2
	lui	$2,%hi(mcache)
	addu	$3,$3,$4
	andi	$3,$3,0xf
	addiu	$2,$2,%lo(mcache)
	sll	$3,$3,2
	addu	$3,$3,$2
	lw	$3,0($3)
	beq	$3,$0,$L1159
	move	$2,$0

	j	$L1175
	lw	$2,0($3)

$L1165:
	lw	$2,0($3)
$L1175:
	beq	$2,$6,$L1163
	nop

	lw	$3,8($3)
	bne	$3,$0,$L1165
	move	$2,$0

	j	$L1159
	nop

$L1163:
	beq	$3,$0,$L1176
	move	$2,$0

	lw	$3,4($3)
$L1174:
	beq	$3,$0,$L1166
	move	$2,$0

	lw	$2,0($3)
	beq	$2,$5,$L1176
	move	$2,$3

	j	$L1174
	lw	$3,12($3)

$L1166:
$L1159:
$L1176:
	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	find_server_cache
	.align	2
	.globl	iptv_FindIgmpGroup
	.ent	iptv_FindIgmpGroup
	.type	iptv_FindIgmpGroup, @function
iptv_FindIgmpGroup:
	.frame	$sp,8,$31		# vars= 8, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-8
	sw	$4,0($sp)
	lbu	$2,2($sp)
	lbu	$3,1($sp)
	move	$5,$4
	lbu	$4,3($sp)
	addu	$3,$3,$2
	lui	$2,%hi(iptvGroup)
	addu	$3,$3,$4
	andi	$3,$3,0xf
	sll	$3,$3,2
	addiu	$2,$2,%lo(iptvGroup)
	addu	$3,$3,$2
	lw	$3,0($3)
$L1185:
	beq	$3,$0,$L1184
	move	$2,$0

	lw	$2,0($3)
	beq	$2,$5,$L1177
	move	$2,$3

	j	$L1185
	lw	$3,464($3)

$L1184:
$L1177:
	j	$31
	addiu	$sp,$sp,8

	.set	macro
	.set	reorder
	.end	iptv_FindIgmpGroup
	.section	.rodata.str1.4
	.align	2
$LC41:
	.ascii	"In group [%d.%d.%d.%d], we added the server cache[%d.%d."
	.ascii	"%d.%d] port_list[%d]\n\000"
	.align	2
$LC42:
	.ascii	"2. Add group [%d.%d.%d.%d] server cache[%d.%d.%d.%d] por"
	.ascii	"t_list[%d]\n\000"
	.align	2
$LC40:
	.ascii	"In group [%d.%d.%d.%d], the server cache[%d.%d.%d.%d] po"
	.ascii	"rt_list[%d] is existence\n\000"
	.text
	.align	2
	.globl	iptv_add_server_cache
	.ent	iptv_add_server_cache
	.type	iptv_add_server_cache, @function
iptv_add_server_cache:
	.frame	$sp,72,$31		# vars= 8, regs= 6/0, args= 40, gp= 0
	.mask	0x801f0000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-72
	sw	$31,68($sp)
	sw	$18,56($sp)
	sw	$16,48($sp)
	sw	$4,72($sp)
	sw	$5,76($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	jal	add_server_cache
	sw	$17,52($sp)

	lw	$4,72($sp)
	jal	iptv_FindIgmpGroup
	move	$16,$2

	beq	$2,$0,$L1186
	move	$18,$2

	bne	$16,$0,$L1324
	lw	$6,72($sp)

	lui	$20,%hi(mcache)
	addiu	$5,$20,%lo(mcache)
	sw	$6,40($sp)
	lbu	$2,41($sp)
	lbu	$3,42($sp)
	lbu	$4,43($sp)
	addu	$2,$2,$3
	addu	$2,$2,$4
	andi	$2,$2,0xf
	sll	$19,$2,2
	addu	$5,$19,$5
	lw	$4,0($5)
$L1331:
	beq	$4,$0,$L1190
	nop

	lw	$2,0($4)
	beq	$2,$6,$L1192
	nop

	j	$L1331
	lw	$4,8($4)

$L1192:
	beq	$4,$0,$L1334
	lui	$16,%hi(malloc_sizes)

	lw	$16,4($4)
	beq	$16,$0,$L1327
	lw	$3,76($sp)

$L1200:
	lw	$2,0($16)
	beq	$2,$3,$L1330
	move	$17,$16

	lw	$16,12($16)
	bne	$16,$0,$L1200
	nop

$L1327:
$L1203:
	lui	$2,%hi(malloc_sizes+4)
	lw	$4,%lo(malloc_sizes+4)($2)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	move	$16,$2
	beq	$2,$0,$L1190
	sw	$2,12($17)

	move	$4,$2
	move	$5,$0
	jal	memset
	li	$6,16			# 0x10

	lw	$2,76($sp)
	lui	$4,%hi($LC41)
	addiu	$4,$4,%lo($LC41)
	sw	$2,0($16)
	lbu	$2,455($18)
	sw	$2,8($16)
	lbu	$2,75($sp)
	lbu	$5,77($sp)
	lbu	$6,78($sp)
	lbu	$7,79($sp)
	lbu	$3,76($sp)
	sw	$5,24($sp)
	sw	$6,28($sp)
	sw	$7,32($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lw	$2,8($16)
	lbu	$5,72($sp)
	lbu	$6,73($sp)
	lbu	$7,74($sp)
	jal	printk
	sw	$2,36($sp)

	li	$2,129			# 0x81
	j	$L1199
	sw	$2,4($16)

$L1190:
$L1241:
$L1278:
	lui	$16,%hi(malloc_sizes)
$L1334:
	addiu	$16,$16,%lo(malloc_sizes)
	lw	$4,4($16)
	jal	kmem_cache_alloc
	li	$5,32			# 0x20

	lw	$4,4($16)
	li	$5,32			# 0x20
	jal	kmem_cache_alloc
	move	$17,$2

	beq	$17,$0,$L1199
	move	$16,$2

	beq	$2,$0,$L1314
	move	$4,$17

	move	$5,$0
	jal	memset
	li	$6,12			# 0xc

	move	$4,$16
	move	$5,$0
	jal	memset
	li	$6,16			# 0x10

	lw	$2,72($sp)
	lui	$4,%hi($LC42)
	addiu	$4,$4,%lo($LC42)
	sw	$2,0($17)
	lw	$2,76($sp)
	sw	$2,0($16)
	lbu	$2,455($18)
	sw	$2,8($16)
	lbu	$2,75($sp)
	lbu	$3,76($sp)
	lbu	$5,77($sp)
	lbu	$6,78($sp)
	lbu	$7,79($sp)
	sw	$3,20($sp)
	sw	$5,24($sp)
	sw	$6,28($sp)
	sw	$7,32($sp)
	sw	$2,16($sp)
	lw	$2,8($16)
	lbu	$5,72($sp)
	lbu	$6,73($sp)
	lbu	$7,74($sp)
	jal	printk
	sw	$2,36($sp)

	li	$2,129			# 0x81
	addiu	$3,$20,%lo(mcache)
	sw	$2,4($16)
	addu	$2,$19,$3
	sw	$16,4($17)
	lw	$4,0($2)
	beq	$4,$0,$L1315
	move	$3,$4

$L1332:
	lw	$2,8($3)
	beq	$2,$0,$L1329
	nop

	j	$L1332
	lw	$3,8($3)

$L1329:
	j	$L1199
	sw	$17,8($3)

$L1315:
	j	$L1199
	sw	$17,0($2)

$L1330:
	lbu	$2,75($sp)
	lbu	$4,77($sp)
	lbu	$5,78($sp)
	lbu	$6,79($sp)
	lbu	$3,76($sp)
	sw	$4,24($sp)
	sw	$5,28($sp)
	sw	$6,32($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lw	$2,8($16)
	lbu	$5,72($sp)
	lbu	$6,73($sp)
	lbu	$7,74($sp)
	lui	$4,%hi($LC40)
	addiu	$4,$4,%lo($LC40)
	jal	printk
	sw	$2,36($sp)

	j	$L1199
	nop

$L1314:
	jal	kfree
	move	$4,$17

	j	$L1333
	move	$2,$16

$L1199:
	beq	$16,$0,$L1333
	move	$2,$16

$L1324:
	lbu	$3,455($18)
	lw	$4,8($16)
	and	$2,$4,$3
	beq	$2,$3,$L1186
	or	$2,$4,$3

	sw	$2,8($16)
$L1186:
	move	$2,$16
$L1333:
	lw	$31,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
	.end	iptv_add_server_cache
	.align	2
	.globl	iptv_br_multicast
	.ent	iptv_br_multicast
	.type	iptv_br_multicast, @function
iptv_br_multicast:
	.frame	$sp,96,$31		# vars= 40, regs= 8/0, args= 24, gp= 0
	.mask	0x807f0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-96
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$31,92($sp)
	sw	$22,88($sp)
	sw	$21,84($sp)
	sw	$20,80($sp)
	sw	$16,64($sp)
	lw	$20,32($5)
	sb	$0,48($sp)
	move	$17,$5
	lw	$5,36($5)
	move	$18,$4
	move	$19,$6
	addiu	$4,$sp,40
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	.set	noreorder
	.set	nomacro
	jal	find_group_info
	lw	$4,16($20)
	.set	macro
	.set	reorder

	lw	$4,16($20)
	.set	noreorder
	.set	nomacro
	jal	iptv_FindIgmpGroup
	move	$16,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1341
	move	$21,$2
	.set	macro
	.set	reorder

	bne	$2,$0,$L1341
	bne	$19,$0,$L1429
	lw	$2,52($18)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L1467
	sw	$2,52($18)
	.set	macro
	.set	reorder

$L1341:
	lw	$4,16($20)
	.set	noreorder
	.set	nomacro
	jal	find_server_cache
	lw	$5,12($20)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1463
	move	$22,$2
	.set	macro
	.set	reorder

	lw	$4,16($20)
	.set	noreorder
	.set	nomacro
	jal	iptv_add_server_cache
	lw	$5,12($20)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1356
	move	$22,$2
	.set	macro
	.set	reorder

$L1463:
	addiu	$4,$sp,24
	move	$5,$22
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

	j	$L1359
$L1356:
	addiu	$4,$sp,24
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,16			# 0x10
	.set	macro
	.set	reorder

$L1359:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L1360
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	skb_clone
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1360
	move	$17,$2
	.set	macro
	.set	reorder

	lw	$2,52($18)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L1335
	sw	$2,52($18)
	.set	macro
	.set	reorder

$L1360:
	lw	$2,140($17)
	lw	$3,88($17)
	lw	$4,136($17)
	addiu	$2,$2,-14
	addiu	$3,$3,14
	sltu	$4,$2,$4
	sw	$3,88($17)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L1362
	sw	$2,140($17)
	.set	macro
	.set	reorder

$L1363:
	lui	$6,%hi($L1363)
	addiu	$6,$6,%lo($L1363)
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	skb_under_panic
	li	$5,14			# 0xe
	.set	macro
	.set	reorder

$L1362:
	lw	$2,32($sp)
	andi	$2,$2,0x80
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1469
	lw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	beq	$16,$0,$L1371
	lw	$18,4($16)
$L1464:
	beq	$18,$0,$L1371
	lw	$3,52($18)
	lw	$2,20($17)
	beq	$3,$2,$L1374
	lw	$2,16($18)
	lw	$6,12($20)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1375
	lw	$3,12($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	blez	$3,$L1462
	move	$4,$0
	.set	macro
	.set	reorder

	move	$5,$3
	addiu	$3,$18,20
$L1381:
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	beq	$6,$2,$L1382
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	addiu	$4,$4,1
	slt	$2,$4,$5
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1381
	addiu	$3,$3,4
	.set	macro
	.set	reorder

$L1462:
	.set	noreorder
	.set	nomacro
	j	$L1382
	move	$2,$0
	.set	macro
	.set	reorder

$L1375:
	.set	noreorder
	.set	nomacro
	blez	$3,$L1455
	move	$4,$0
	.set	macro
	.set	reorder

	move	$5,$3
	addiu	$3,$18,20
$L1389:
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	beq	$6,$2,$L1382
	move	$2,$0
	.set	macro
	.set	reorder

	addiu	$4,$4,1
	slt	$2,$4,$5
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1389
	addiu	$3,$3,4
	.set	macro
	.set	reorder

$L1455:
	li	$2,1			# 0x1
$L1382:
	beq	$2,$0,$L1374
	move	$4,$17
	.set	noreorder
	.set	nomacro
	jal	pskb_copy
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1371
	move	$16,$2
	.set	macro
	.set	reorder

	lw	$4,140($2)
	move	$5,$18
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$2,52($18)
	sw	$2,20($16)
	lw	$5,52($18)
	lw	$2,308($5)
	.set	noreorder
	.set	nomacro
	jal	$2
	move	$4,$16
	.set	macro
	.set	reorder

$L1374:
	.set	noreorder
	.set	nomacro
	j	$L1464
	lw	$18,56($18)
	.set	macro
	.set	reorder

$L1371:
	beq	$21,$0,$L1405
	move	$19,$0
	move	$18,$21
$L1403:
	lbu	$2,10($18)
	beq	$2,$0,$L1397
	lb	$2,11($18)
	bgez	$2,$L1397
	lw	$3,20($17)
	lw	$2,12($18)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L1397
	move	$5,$21
	.set	macro
	.set	reorder

	lw	$4,12($20)
	addiu	$6,$sp,48
	move	$7,$19
	.set	noreorder
	.set	nomacro
	jal	iptv_wireless_multicast_filter
	sw	$17,16($sp)
	.set	macro
	.set	reorder

	lbu	$3,48($sp)
	li	$2,128			# 0x80
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1397
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	pskb_copy
	move	$4,$17
	.set	macro
	.set	reorder

	move	$16,$2
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1405
	addiu	$5,$18,4
	.set	macro
	.set	reorder

	lw	$4,140($2)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	lw	$5,12($18)
	sw	$5,20($16)
	lw	$2,308($5)
	.set	noreorder
	.set	nomacro
	jal	$2
	move	$4,$16
	.set	macro
	.set	reorder

	sb	$0,48($sp)
$L1397:
	addiu	$19,$19,1
	slt	$2,$19,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1403
	addiu	$18,$18,56
	.set	macro
	.set	reorder

$L1405:
	jal	local_bh_enable
	lw	$2,32($sp)
$L1469:
	andi	$2,$2,0x7f
	beq	$2,$0,$L1406
	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L1412
	move	$19,$0
	.set	macro
	.set	reorder

$L1416:
	lw	$4,12($20)
	move	$6,$19
	move	$5,$21
	addiu	$19,$19,1
	.set	noreorder
	.set	nomacro
	jal	iptv_wire_multicast_filter
	addiu	$7,$sp,24
	.set	macro
	.set	reorder

	slt	$2,$19,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1416
	lw	$2,32($sp)
	.set	macro
	.set	reorder

	sw	$2,8($22)
	lw	$2,32($sp)
	andi	$2,$2,0x7f
	bne	$2,$0,$L1412
$L1467:
	lw	$3,132($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L1418
	j	$L1419
$L1418:
#APP
		.set	mips3					
1:	ll	$3, 132($17)		# atomic_sub_return	
	subu	$2, $3, 1				
	sc	$2, 132($17)					
	beqz	$2, 1b					
	subu	$2, $3, 1				
	sync						
	.set	mips0					

#NO_APP
	bne	$2,$0,$L1429
$L1419:
	.set	noreorder
	.set	nomacro
	jal	__kfree_skb
	move	$4,$17
	.set	macro
	.set	reorder

$L1429:
	jal	local_bh_enable
	.set	noreorder
	.set	nomacro
	j	$L1468
	lw	$31,92($sp)
	.set	macro
	.set	reorder

$L1412:
	move	$19,$0
	lw	$2,32($sp)
$L1470:
	sra	$2,$2,$19
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1432
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	pskb_copy
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1432
	move	$4,$2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$19,$0,$L1435
	lui	$2,%hi(Eth1dev)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1465
	lw	$2,%lo(Eth1dev)($2)
	.set	macro
	.set	reorder

$L1435:
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$19,$2,$L1436
	lui	$2,%hi(Eth2dev)
	.set	macro
	.set	reorder

	lw	$2,%lo(Eth2dev)($2)
$L1465:
	sw	$2,20($4)
$L1436:
	lw	$5,20($4)
	lw	$2,308($5)
	jal	$2
$L1432:
	addiu	$19,$19,1
	slt	$2,$19,2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1470
	lw	$2,32($sp)
	.set	macro
	.set	reorder

	jal	local_bh_enable
$L1406:
	lw	$3,132($17)
	li	$2,1			# 0x1
	bne	$3,$2,$L1441
	j	$L1442
$L1441:
#APP
		.set	mips3					
1:	ll	$3, 132($17)		# atomic_sub_return	
	subu	$2, $3, 1				
	sc	$2, 132($17)					
	beqz	$2, 1b					
	subu	$2, $3, 1				
	sync						
	.set	mips0					

#NO_APP
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1468
	lw	$31,92($sp)
	.set	macro
	.set	reorder

$L1442:
	.set	noreorder
	.set	nomacro
	jal	__kfree_skb
	move	$4,$17
	.set	macro
	.set	reorder

$L1335:
	lw	$31,92($sp)
$L1468:
	lw	$22,88($sp)
	lw	$21,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,96
	.set	macro
	.set	reorder

	.end	iptv_br_multicast
	.section	.rodata.str1.4
	.align	2
$LC43:
	.ascii	"iptv_LeaveIgmpGroup: remove port[%d] from group[%d.%d.%d"
	.ascii	".%d] by host[%d.%d.%d.%d]\n\000"
	.align	2
$LC44:
	.ascii	"iptv_LeaveIgmpGroup: group[%d.%d.%d.%d] is removed\n\000"
	.text
	.align	2
	.ent	iptv_LeaveIgmpGroup
	.type	iptv_LeaveIgmpGroup, @function
iptv_LeaveIgmpGroup:
	.frame	$sp,112,$31		# vars= 32, regs= 10/0, args= 40, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-112
	sw	$5,40($sp)
	sw	$31,108($sp)
	sw	$fp,104($sp)
	sw	$23,100($sp)
	sw	$22,96($sp)
	sw	$21,92($sp)
	sw	$20,88($sp)
	sw	$19,84($sp)
	sw	$18,80($sp)
	sw	$17,76($sp)
	sw	$16,72($sp)
	move	$7,$4
	lw	$4,20($4)
	lbu	$2,41($sp)
	sw	$5,116($sp)
	lbu	$6,42($sp)
	lb	$5,0($4)
	lbu	$3,43($sp)
	lw	$4,36($7)
	addu	$2,$2,$6
	addu	$2,$2,$3
	addiu	$4,$4,6
	andi	$3,$2,0xf
	sw	$4,56($sp)
	li	$2,128			# 0x80
	sw	$0,52($sp)
	lw	$22,32($7)
	sw	$2,60($sp)
	li	$2,97			# 0x61
	beq	$5,$2,$L1474
	lbu	$7,51($7)
	sw	$7,60($sp)
$L1474:
	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	sll	$3,$3,2
	sw	$3,64($sp)
	lw	$4,64($sp)
	lui	$3,%hi(iptvGroup)
	addiu	$2,$3,%lo(iptvGroup)
	addu	$2,$4,$2
	lw	$19,0($2)
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L1488
	lw	$3,116($sp)
	.set	macro
	.set	reorder

$L1483:
	lw	$2,0($19)
	beq	$2,$3,$L1481
	sw	$19,52($sp)
	lw	$19,464($19)
	bne	$19,$0,$L1483
	j	$L1488
$L1481:
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L1488
	move	$fp,$0
	.set	macro
	.set	reorder

	sw	$0,460($19)
	move	$20,$19
$L1516:
	lbu	$2,10($20)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1489
	lw	$5,56($sp)
	.set	macro
	.set	reorder

	addiu	$4,$20,4
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1491
	move	$3,$0
	.set	macro
	.set	reorder

	sb	$0,10($20)
	lw	$4,60($sp)
	sra	$2,$4,$3
$L1528:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1527
	addiu	$2,$3,1
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	slt	$2,$3,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1528
	sra	$2,$4,$3
	.set	macro
	.set	reorder

	addiu	$2,$3,1
$L1527:
	andi	$23,$2,0x7
	addu	$4,$23,$19
	lbu	$2,452($4)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1499
	addiu	$2,$2,-1
	.set	macro
	.set	reorder

	sll	$3,$2,24
	sra	$3,$3,24
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1499
	sb	$2,452($4)
	.set	macro
	.set	reorder

	lw	$4,60($sp)
	lbu	$3,455($19)
	nor	$2,$0,$4
	and	$2,$2,$3
	sb	$2,455($19)
	lw	$6,116($sp)
	lui	$2,%hi(mcache)
	addiu	$3,$2,%lo(mcache)
	sw	$6,44($sp)
	lbu	$2,45($sp)
	lbu	$4,46($sp)
	lbu	$5,47($sp)
	sw	$6,48($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$21,$2,2
	addu	$3,$21,$3
	lw	$18,0($3)
	sw	$0,68($sp)
$L1526:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L1529
	lbu	$2,118($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	beq	$2,$6,$L1502
	sw	$18,68($sp)
	.set	noreorder
	.set	nomacro
	j	$L1526
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1502:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L1529
	lbu	$2,118($sp)
	.set	macro
	.set	reorder

	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L1530
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L1508:
	lbu	$2,51($sp)
	lbu	$5,48($sp)
	lbu	$6,49($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,50($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1508
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L1530:
	addiu	$2,$3,%lo(mcache)
	addu	$3,$21,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1509
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1510
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1509:
	lw	$2,68($sp)
	sw	$4,8($2)
$L1510:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

	lbu	$2,118($sp)
$L1529:
	lbu	$3,119($sp)
	lbu	$6,116($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lbu	$2,12($22)
	lbu	$7,117($sp)
	lui	$4,%hi($LC43)
	sw	$2,24($sp)
	lbu	$2,13($22)
	addiu	$4,$4,%lo($LC43)
	move	$5,$23
	sw	$2,28($sp)
	lbu	$2,14($22)
	sw	$2,32($sp)
	lbu	$2,15($22)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,36($sp)
	.set	macro
	.set	reorder

$L1499:
	sw	$0,56($20)
	lbu	$2,455($19)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1491
	lui	$3,%hi(iptvGroup)
	.set	macro
	.set	reorder

	addiu	$2,$3,%lo(iptvGroup)
	lw	$3,64($sp)
	addu	$4,$3,$2
	lw	$2,0($4)
	.set	noreorder
	.set	nomacro
	bne	$19,$2,$L1513
	lw	$3,464($19)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1514
	sw	$3,0($4)
	.set	macro
	.set	reorder

$L1513:
	lw	$4,52($sp)
	sw	$3,464($4)
$L1514:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$19
	.set	macro
	.set	reorder

	lbu	$2,119($sp)
	lbu	$5,116($sp)
	lbu	$6,117($sp)
	lbu	$7,118($sp)
	lui	$4,%hi($LC44)
	addiu	$4,$4,%lo($LC44)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	j	$L1488
$L1491:
	lw	$2,460($19)
	lw	$3,56($20)
	sltu	$2,$2,$3
	beq	$2,$0,$L1489
	sw	$3,460($19)
$L1489:
	addiu	$fp,$fp,1
	slt	$2,$fp,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1516
	addiu	$20,$20,56
	.set	macro
	.set	reorder

$L1488:
	jal	local_bh_enable
	lw	$31,108($sp)
	lw	$fp,104($sp)
	lw	$23,100($sp)
	lw	$22,96($sp)
	lw	$21,92($sp)
	lw	$20,88($sp)
	lw	$19,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,112
	.set	macro
	.set	reorder

	.end	iptv_LeaveIgmpGroup
	.section	.rodata.str1.4
	.align	2
$LC45:
	.ascii	"iptv_AddIgmpGroup: old port[%d] of group[%d.%d.%d.%d] po"
	.ascii	"rt is moved\n\000"
	.align	2
$LC46:
	.ascii	"iptv_AddIgmpGroup: new port[%d] to group[%d.%d.%d.%d] fr"
	.ascii	"om host[%d.%d.%d.%d]\n\000"
	.align	2
$LC47:
	.ascii	"iptv_AddIgmpGroup: new port[%d] to existing group[%d.%d."
	.ascii	"%d.%d] from host [%d.%d.%d.%d]\n\000"
	.align	2
$LC48:
	.ascii	"iptv_AddIgmpGroup: new group[%d.%d.%d.%d] and port[%d] f"
	.ascii	"rom host[%d.%d.%d.%d]\n\000"
	.text
	.align	2
	.ent	iptv_AddIgmpGroup
	.type	iptv_AddIgmpGroup, @function
iptv_AddIgmpGroup:
	.frame	$sp,136,$31		# vars= 56, regs= 10/0, args= 40, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-136
	sw	$4,136($sp)
	sw	$31,132($sp)
	sw	$fp,128($sp)
	sw	$23,124($sp)
	sw	$22,120($sp)
	sw	$21,116($sp)
	sw	$20,112($sp)
	sw	$19,108($sp)
	sw	$18,104($sp)
	sw	$17,100($sp)
	sw	$16,96($sp)
	lw	$2,20($4)
	sw	$5,140($sp)
	sw	$5,40($sp)
	move	$4,$5
	lw	$5,136($sp)
	lb	$3,0($2)
	lw	$2,36($5)
	lw	$23,32($5)
	addiu	$2,$2,6
	sw	$2,84($sp)
	li	$2,128			# 0x80
	sw	$2,88($sp)
	li	$2,97			# 0x61
	beq	$3,$2,$L1534
	lbu	$3,51($5)
	sw	$3,88($sp)
$L1534:
	sw	$4,44($sp)
	lbu	$2,45($sp)
	lbu	$4,46($sp)
	lbu	$5,47($sp)
	lw	$3,20($28)
	addu	$2,$2,$4
	addu	$2,$2,$5
	addiu	$3,$3,256
	sw	$3,20($28)
	andi	$2,$2,0xf
	lui	$18,%hi(iptvGroup)
	sll	$17,$2,2
	addiu	$2,$18,%lo(iptvGroup)
	addu	$2,$17,$2
	lw	$19,0($2)
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L1545
	lw	$3,140($sp)
	.set	macro
	.set	reorder

$L1544:
	lw	$2,0($19)
	beq	$2,$3,$L1542
	lw	$19,464($19)
	.set	noreorder
	.set	nomacro
	bne	$19,$0,$L1544
	lui	$2,%hi(malloc_sizes+76)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1729
	lw	$4,%lo(malloc_sizes+76)($2)
	.set	macro
	.set	reorder

$L1542:
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L1545
	move	$16,$0
	.set	macro
	.set	reorder

	sw	$0,460($19)
	sw	$0,80($sp)
	move	$20,$19
$L1591:
	lbu	$2,10($20)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1713
	lw	$5,80($sp)
	.set	macro
	.set	reorder

	lw	$5,84($sp)
	addiu	$4,$20,4
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1550
	lw	$4,88($sp)
	.set	macro
	.set	reorder

	andi	$2,$4,0x7f
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1714
	lw	$4,136($sp)
	.set	macro
	.set	reorder

	lbu	$4,11($20)
	lw	$5,88($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$5,$L1551
	move	$3,$0
	.set	macro
	.set	reorder

	sra	$2,$4,$3
$L1716:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1715
	addiu	$2,$3,1
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	slt	$2,$3,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1716
	sra	$2,$4,$3
	.set	macro
	.set	reorder

	addiu	$2,$3,1
$L1715:
	andi	$21,$2,0x00ff
	addu	$4,$21,$19
	lbu	$2,452($4)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1558
	addiu	$2,$2,-1
	.set	macro
	.set	reorder

	sll	$3,$2,24
	sra	$3,$3,24
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1558
	sb	$2,452($4)
	.set	macro
	.set	reorder

	lbu	$2,11($20)
	lbu	$3,455($19)
	move	$fp,$0
	nor	$2,$0,$2
	and	$2,$2,$3
	sb	$2,455($19)
	lw	$6,140($sp)
	lui	$2,%hi(mcache)
	addiu	$3,$2,%lo(mcache)
	sw	$6,48($sp)
	lbu	$2,49($sp)
	lbu	$4,50($sp)
	lbu	$5,51($sp)
	sw	$6,52($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$22,$2,2
	addu	$3,$22,$3
	lw	$18,0($3)
$L1707:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L1717
	lbu	$2,142($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	beq	$2,$6,$L1561
	move	$fp,$18
	.set	noreorder
	.set	nomacro
	j	$L1707
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1561:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L1717
	lbu	$2,142($sp)
	.set	macro
	.set	reorder

	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L1718
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L1567:
	lbu	$2,55($sp)
	lbu	$5,52($sp)
	lbu	$6,53($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,54($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1567
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L1718:
	addiu	$2,$3,%lo(mcache)
	addu	$3,$22,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1568
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1569
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1568:
	sw	$4,8($fp)
$L1569:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

	lbu	$2,142($sp)
$L1717:
	lbu	$3,143($sp)
	lbu	$6,140($sp)
	lbu	$7,141($sp)
	lui	$4,%hi($LC45)
	addiu	$4,$4,%lo($LC45)
	move	$5,$21
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$3,20($sp)
	.set	macro
	.set	reorder

$L1558:
	lw	$4,88($sp)
	move	$3,$0
	sb	$4,11($20)
	lw	$5,88($sp)
	sra	$2,$5,$3
$L1720:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1719
	addiu	$2,$3,1
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	slt	$2,$3,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1720
	sra	$2,$5,$3
	.set	macro
	.set	reorder

	addiu	$2,$3,1
$L1719:
	andi	$21,$2,0x00ff
	addu	$22,$21,$19
	lb	$2,452($22)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1577
	lw	$6,140($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(mcache)
	addiu	$3,$2,%lo(mcache)
	sw	$6,56($sp)
	lbu	$2,57($sp)
	lbu	$4,58($sp)
	lbu	$5,59($sp)
	sw	$6,60($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$fp,$2,2
	addu	$3,$fp,$3
	lw	$18,0($3)
	sw	$0,92($sp)
$L1708:
	beq	$18,$0,$L1589
	lw	$2,0($18)
	beq	$2,$6,$L1580
	sw	$18,92($sp)
	.set	noreorder
	.set	nomacro
	j	$L1708
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1580:
	beq	$18,$0,$L1589
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L1721
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L1586:
	lbu	$2,63($sp)
	lbu	$5,60($sp)
	lbu	$6,61($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,62($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1586
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L1721:
	addiu	$2,$3,%lo(mcache)
	addu	$3,$fp,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1587
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1588
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1587:
	lw	$5,92($sp)
	sw	$4,8($5)
$L1588:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L1589:
	lbu	$2,455($19)
	lw	$3,88($sp)
	lui	$4,%hi($LC46)
	addiu	$4,$4,%lo($LC46)
	or	$2,$3,$2
	sb	$2,455($19)
	lbu	$2,142($sp)
	lbu	$3,143($sp)
	lbu	$6,140($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lbu	$2,12($23)
	lbu	$7,141($sp)
	move	$5,$21
	sw	$2,24($sp)
	lbu	$2,13($23)
	sw	$2,28($sp)
	lbu	$2,14($23)
	sw	$2,32($sp)
	lbu	$2,15($23)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,36($sp)
	.set	macro
	.set	reorder

$L1577:
	lbu	$2,452($22)
	addiu	$2,$2,1
	sb	$2,452($22)
$L1551:
	lw	$4,136($sp)
$L1714:
	li	$16,1			# 0x1
	lw	$2,20($4)
	sw	$0,56($20)
	sw	$2,12($20)
$L1550:
	lw	$2,460($19)
	lw	$3,56($20)
	sltu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1713
	lw	$5,80($sp)
	.set	macro
	.set	reorder

	sw	$3,460($19)
	lw	$5,80($sp)
$L1713:
	addiu	$20,$20,56
	addiu	$5,$5,1
	slt	$2,$5,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1591
	sw	$5,80($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1684
	move	$16,$19
	.set	macro
	.set	reorder

	sw	$0,80($sp)
$L1618:
	lw	$2,80($sp)
	addiu	$2,$2,1
	sw	$2,80($sp)
	lw	$4,80($sp)
	lbu	$2,10($16)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1597
	slt	$3,$4,8
	.set	macro
	.set	reorder

	li	$2,1
	sb	$2,10($16)
	lw	$5,88($sp)
	addiu	$4,$16,4
	li	$6,6			# 0x6
	sb	$5,11($16)
	lw	$3,136($sp)
	lw	$5,84($sp)
	lw	$2,20($3)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,12($16)
	.set	macro
	.set	reorder

	li	$6,40			# 0x28
	addiu	$4,$16,16
	.set	noreorder
	.set	nomacro
	jal	memset
	move	$5,$0
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	sw	$2,20($16)
	sw	$0,56($16)
	move	$6,$0
	lw	$4,88($sp)
	sra	$2,$4,$6
$L1723:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1722
	lw	$7,140($sp)
	.set	macro
	.set	reorder

	addiu	$6,$6,1
	slt	$2,$6,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1723
	sra	$2,$4,$6
	.set	macro
	.set	reorder

	lw	$7,140($sp)
$L1722:
	lui	$fp,%hi(mcache)
	addiu	$3,$fp,%lo(mcache)
	sw	$7,64($sp)
	lbu	$2,65($sp)
	lbu	$4,66($sp)
	lbu	$5,67($sp)
	addiu	$6,$6,1
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$20,$2,2
	addu	$3,$20,$3
	lw	$18,0($3)
	andi	$21,$6,0x7
	sw	$7,68($sp)
	move	$22,$0
$L1709:
	beq	$18,$0,$L1617
	lw	$2,0($18)
	beq	$2,$7,$L1608
	move	$22,$18
	.set	noreorder
	.set	nomacro
	j	$L1709
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1608:
	beq	$18,$0,$L1617
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L1724
	addiu	$2,$fp,%lo(mcache)
	.set	macro
	.set	reorder

$L1614:
	lbu	$2,71($sp)
	lbu	$5,68($sp)
	lbu	$6,69($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,70($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1614
	move	$17,$16
	.set	macro
	.set	reorder

	addiu	$2,$fp,%lo(mcache)
$L1724:
	addu	$3,$20,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1615
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1616
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1615:
	sw	$4,8($22)
$L1616:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L1617:
	lbu	$2,455($19)
	lw	$5,88($sp)
	lui	$4,%hi($LC47)
	addiu	$4,$4,%lo($LC47)
	or	$2,$5,$2
	sb	$2,455($19)
	lbu	$2,142($sp)
	lbu	$3,143($sp)
	lbu	$6,140($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lbu	$2,12($23)
	lbu	$7,141($sp)
	move	$5,$21
	sw	$2,24($sp)
	lbu	$2,13($23)
	sw	$2,28($sp)
	lbu	$2,14($23)
	sw	$2,32($sp)
	lbu	$2,15($23)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,36($sp)
	.set	macro
	.set	reorder

	addu	$3,$21,$19
	lbu	$2,452($3)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L1684
	sb	$2,452($3)
	.set	macro
	.set	reorder

$L1597:
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1618
	addiu	$16,$16,56
	.set	macro
	.set	reorder

	j	$L1684
$L1545:
$L1622:
	lui	$2,%hi(malloc_sizes+76)
	lw	$4,%lo(malloc_sizes+76)($2)
$L1729:
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1684
	move	$16,$2
	.set	macro
	.set	reorder

	move	$4,$2
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,468			# 0x1d4
	.set	macro
	.set	reorder

	lw	$3,140($sp)
	li	$2,1
	sb	$2,10($16)
	sw	$3,0($16)
	lw	$2,88($sp)
	addiu	$4,$16,4
	li	$6,6			# 0x6
	sb	$2,11($16)
	lw	$3,136($sp)
	lw	$5,84($sp)
	lw	$2,20($3)
	sw	$0,56($16)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,12($16)
	.set	macro
	.set	reorder

	addiu	$4,$16,16
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	sw	$2,20($16)
	move	$4,$0
	lw	$5,88($sp)
	sra	$2,$5,$4
$L1726:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1725
	addiu	$2,$4,1
	.set	macro
	.set	reorder

	addiu	$4,$4,1
	slt	$2,$4,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1726
	sra	$2,$5,$4
	.set	macro
	.set	reorder

	addiu	$2,$4,1
$L1725:
	lbu	$3,455($16)
	andi	$21,$2,0x7
	lw	$2,88($sp)
	addiu	$5,$18,%lo(iptvGroup)
	addu	$4,$21,$16
	or	$3,$2,$3
	li	$2,1
	sb	$2,452($4)
	sb	$3,455($16)
	addu	$5,$17,$5
	sw	$0,460($16)
	sw	$0,464($16)
	lw	$2,0($5)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1665
	move	$19,$2
	.set	macro
	.set	reorder

$L1710:
	lw	$2,464($19)
	beq	$2,$0,$L1703
	.set	noreorder
	.set	nomacro
	j	$L1710
	lw	$19,464($19)
	.set	macro
	.set	reorder

$L1703:
	.set	noreorder
	.set	nomacro
	j	$L1670
	sw	$16,464($19)
	.set	macro
	.set	reorder

$L1665:
	sw	$16,0($5)
$L1670:
	lw	$6,140($sp)
	lui	$22,%hi(mcache)
	addiu	$3,$22,%lo(mcache)
	sw	$6,72($sp)
	lbu	$2,73($sp)
	lbu	$4,74($sp)
	lbu	$5,75($sp)
	sw	$6,76($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$19,$2,2
	addu	$3,$19,$3
	lw	$18,0($3)
	move	$20,$0
$L1711:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L1727
	lbu	$2,143($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	beq	$2,$6,$L1673
	move	$20,$18
	.set	noreorder
	.set	nomacro
	j	$L1711
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1673:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L1727
	lbu	$2,143($sp)
	.set	macro
	.set	reorder

	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L1728
	addiu	$2,$22,%lo(mcache)
	.set	macro
	.set	reorder

$L1679:
	lbu	$2,79($sp)
	lbu	$5,76($sp)
	lbu	$6,77($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,78($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1679
	move	$17,$16
	.set	macro
	.set	reorder

	addiu	$2,$22,%lo(mcache)
$L1728:
	addu	$3,$19,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1680
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1681
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1680:
	sw	$4,8($20)
$L1681:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

	lbu	$2,143($sp)
$L1727:
	sw	$21,20($sp)
	lbu	$5,140($sp)
	sw	$2,16($sp)
	lbu	$2,12($23)
	lbu	$6,141($sp)
	lbu	$7,142($sp)
	sw	$2,24($sp)
	lbu	$2,13($23)
	lui	$4,%hi($LC48)
	addiu	$4,$4,%lo($LC48)
	sw	$2,28($sp)
	lbu	$2,14($23)
	sw	$2,32($sp)
	lbu	$2,15($23)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,36($sp)
	.set	macro
	.set	reorder

$L1684:
	jal	local_bh_enable
	lw	$31,132($sp)
	lw	$fp,128($sp)
	lw	$23,124($sp)
	lw	$22,120($sp)
	lw	$21,116($sp)
	lw	$20,112($sp)
	lw	$19,108($sp)
	lw	$18,104($sp)
	lw	$17,100($sp)
	lw	$16,96($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,136
	.set	macro
	.set	reorder

	.end	iptv_AddIgmpGroup
	.section	.rodata.str1.4
	.align	2
$LC49:
	.ascii	"group record type = %d --------------------------\n\000"
	.align	2
$LC50:
	.ascii	"Leave group [%d.%d.%d.%d] from host [%d.%d.%d.%d]\n\000"
	.align	2
$LC51:
	.ascii	"iptv_AddIgmpV3_Group_Source_list: Found the mgroup [%d.%"
	.ascii	"d.%d.%d]! \n\000"
	.align	2
$LC52:
	.ascii	"iptv_AddIgmpV3_Group_Source_list: before setting: group["
	.ascii	"%d.%d.%d.%d], port[%d] and mode[%d] from host[%d.%d.%d.%"
	.ascii	"d]\n\000"
	.align	2
$LC53:
	.ascii	"In copy_source_list of iptv_AddIgmpV3_Group_Source_list\n"
	.ascii	"\000"
	.align	2
$LC54:
	.ascii	"source number = %d \n\000"
	.align	2
$LC55:
	.ascii	"Source IP [%d.%d.%d.%d]! \n\000"
	.align	2
$LC56:
	.ascii	"In append_new_source_list of iptv_AddIgmpV3_Group_Source"
	.ascii	"_list\n\000"
	.align	2
$LC57:
	.ascii	"Append source IP [%d.%d.%d.%d]! \n\000"
	.align	2
$LC58:
	.ascii	"Member: Source IP [%d.%d.%d.%d]! \n\000"
	.align	2
$LC59:
	.ascii	"Append source ip has been existed in source list! \n\000"
	.align	2
$LC60:
	.ascii	"Append source ip doesn't exist in source list! \n\000"
	.align	2
$LC61:
	.ascii	"Member: After appending of Source IP [%d.%d.%d.%d]! \n\000"
	.align	2
$LC62:
	.ascii	"In delete_some_source_list of iptv_AddIgmpV3_Group_Sourc"
	.ascii	"e_list\n\000"
	.align	2
$LC63:
	.ascii	"Block source IP [%d.%d.%d.%d]! \n\000"
	.align	2
$LC65:
	.ascii	"Block source IP [%d.%d.%d.%d] doesn't find in source lis"
	.ascii	"t! \n\000"
	.align	2
$LC64:
	.ascii	"Found the block source ip! \n\000"
	.align	2
$LC66:
	.ascii	"Member: After sorting of Source IP [%d.%d.%d.%d]! \n\000"
	.align	2
$LC67:
	.ascii	"iptv_AddIgmpV3_Group_Source_list: after setting: group[%"
	.ascii	"d.%d.%d.%d], port[%d] and mode[%d] from host[%d.%d.%d.%d"
	.ascii	"]\n\000"
	.align	2
$LC69:
	.ascii	"Cannot add host[%d.%d.%d.%d] to existing group[%d.%d.%d."
	.ascii	"%d], due to the type is IGMPV3_BLOCK_OLD_SOURCES!\n\000"
	.align	2
$LC68:
	.ascii	"iptv_AddIgmpV3_Group_Source_list: Found the mgroup [%d.%"
	.ascii	"d.%d.%d] but the host doesn't join the group yet! \n\000"
	.align	2
$LC70:
	.ascii	"iptv_AddIgmpGroup: new port[%d] and mode[%d] to existing"
	.ascii	" group[%d.%d.%d.%d] from host [%d.%d.%d.%d]\n\000"
	.align	2
$LC71:
	.ascii	"iptv_AddIgmpV3_Group_Source_list: Not found the mgroup ["
	.ascii	"%d.%d.%d.%d]! \n\000"
	.align	2
$LC72:
	.ascii	"iptv_AddIgmpGroup: new group[%d.%d.%d.%d], port[%d] and "
	.ascii	"mode[%d] from host[%d.%d.%d.%d]\n\000"
	.align	2
$LC73:
	.ascii	"Doesn't create the multicast group[%d.%d.%d.%d] from hos"
	.ascii	"t[%d.%d.%d.%d]!\n\000"
	.align	2
$LC74:
	.ascii	"Add group [%d.%d.%d.%d] from host [%d.%d.%d.%d]\n\000"
	.text
	.align	2
	.globl	iptv_ProcessIgmpFromLan
	.ent	iptv_ProcessIgmpFromLan
	.type	iptv_ProcessIgmpFromLan, @function
iptv_ProcessIgmpFromLan:
	.frame	$sp,288,$31		# vars= 200, regs= 10/0, args= 48, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-288
	sw	$4,288($sp)
	sw	$31,284($sp)
	sw	$fp,280($sp)
	sw	$23,276($sp)
	sw	$22,272($sp)
	sw	$21,268($sp)
	sw	$20,264($sp)
	sw	$19,260($sp)
	sw	$18,256($sp)
	sw	$17,252($sp)
	sw	$16,248($sp)
	lw	$2,32($4)
	sh	$0,112($sp)
	li	$3,-268435456			# 0xfffffffff0000000
	sw	$2,192($sp)
	lw	$2,0($2)
	addiu	$6,$sp,112
	and	$3,$2,$3
	srl	$2,$2,22
	andi	$4,$2,0x3c
	li	$2,1073741824			# 0x40000000
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L1731
	addiu	$7,$sp,120
	.set	macro
	.set	reorder

	sltu	$2,$4,20
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1732
	li	$5,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L1731:
	lw	$5,192($sp)
	lhu	$3,2($5)
	lw	$5,288($sp)
	lw	$2,88($5)
	sltu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2219
	sltu	$2,$3,$4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1732
	li	$5,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lw	$2,192($sp)
	addu	$4,$2,$4
	lbu	$3,0($4)
	li	$2,34			# 0x22
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1735
	addiu	$5,$4,8
	.set	macro
	.set	reorder

	lhu	$2,6($4)
	sltu	$3,$2,17
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1736
	sh	$2,0($6)
	.set	macro
	.set	reorder

	li	$2,16
	sh	$2,0($6)
$L1736:
	lw	$4,4($4)
	move	$22,$5
	sw	$0,196($sp)
	.set	noreorder
	.set	nomacro
	beq	$4,$0,$L2219
	sw	$4,200($sp)
	.set	macro
	.set	reorder

$L2220:
	.set	noreorder
	.set	nomacro
	beq	$22,$0,$L2219
	li	$2,-268435456			# 0xfffffffff0000000
	.set	macro
	.set	reorder

	lw	$6,4($22)
	li	$3,-536870912			# 0xffffffffe0000000
	and	$2,$6,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$3,$L2219
	move	$5,$0
	.set	macro
	.set	reorder

	lhu	$3,2($22)
	sltu	$2,$3,9
	beq	$2,$0,$L2219
	lbu	$2,0($22)
	sltu	$2,$2,7
	beq	$2,$0,$L2219
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L2143
	move	$7,$0
	.set	macro
	.set	reorder

	move	$8,$3
	li	$10,-268435456			# 0xfffffffff0000000
	li	$9,-536870912			# 0xffffffffe0000000
	addiu	$4,$22,8
$L1750:
	lw	$2,0($4)
	addiu	$3,$5,1
	addiu	$7,$7,1
	movz	$5,$3,$2
	and	$2,$2,$10
	xor	$2,$2,$9
	addiu	$3,$5,2
	movz	$5,$3,$2
	slt	$2,$5,2
	slt	$3,$7,$8
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2219
	addiu	$4,$4,4
	.set	macro
	.set	reorder

	bne	$3,$0,$L1750
$L2143:
	li	$2,536870912			# 0x20000000
	addu	$2,$6,$2
	sltu	$2,$2,256
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1739
	sw	$6,136($sp)
	.set	macro
	.set	reorder

	li	$2,-268500992			# 0xffffffffefff0000
	ori	$2,$2,0xfffa
	.set	noreorder
	.set	nomacro
	beq	$6,$2,$L1739
	lui	$4,%hi($LC49)
	.set	macro
	.set	reorder

	lbu	$5,0($22)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC49)
	.set	macro
	.set	reorder

	lbu	$3,0($22)
	sltu	$2,$3,7
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1739
	li	$2,1			# 0x1
	.set	macro
	.set	reorder

	sll	$3,$2,$3
	andi	$2,$3,0xa
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1759
	andi	$2,$3,0x74
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2235
	lw	$4,288($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2229
	lhu	$2,2($22)
	.set	macro
	.set	reorder

$L1759:
	lhu	$2,2($22)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1764
	lbu	$2,139($sp)
	.set	macro
	.set	reorder

	lw	$3,192($sp)
	lbu	$5,136($sp)
	sw	$2,16($sp)
	lbu	$2,12($3)
	lbu	$6,137($sp)
	lbu	$7,138($sp)
	sw	$2,20($sp)
	lbu	$2,13($3)
	lui	$4,%hi($LC50)
	addiu	$4,$4,%lo($LC50)
	sw	$2,24($sp)
	lbu	$2,14($3)
	sw	$2,28($sp)
	lbu	$2,15($3)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$4,288($sp)
	.set	noreorder
	.set	nomacro
	j	$L2228
	lw	$5,136($sp)
	.set	macro
	.set	reorder

$L1764:
	lw	$4,288($sp)
$L2235:
	lw	$6,136($sp)
	lw	$5,288($sp)
	lw	$2,20($4)
	lw	$3,36($4)
	sw	$6,148($sp)
	sw	$6,140($sp)
	lb	$4,0($2)
	addiu	$3,$3,6
	li	$2,128			# 0x80
	sw	$3,208($sp)
	lw	$fp,32($5)
	sw	$2,216($sp)
	li	$2,97			# 0x61
	beq	$4,$2,$L1767
	lbu	$3,51($5)
	sw	$3,216($sp)
$L1767:
	sw	$6,144($sp)
	lbu	$2,145($sp)
	lbu	$4,146($sp)
	lbu	$5,147($sp)
	lw	$3,20($28)
	addu	$2,$2,$4
	addu	$2,$2,$5
	addiu	$3,$3,256
	sw	$3,20($28)
	andi	$2,$2,0xf
	lui	$20,%hi(iptvGroup)
	sll	$18,$2,2
	addiu	$2,$20,%lo(iptvGroup)
	addu	$2,$18,$2
	lw	$21,0($2)
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L1775
	lw	$3,148($sp)
	.set	macro
	.set	reorder

$L1777:
	lw	$2,0($21)
	.set	noreorder
	.set	nomacro
	beq	$2,$3,$L2236
	lbu	$5,148($sp)
	.set	macro
	.set	reorder

	lw	$21,464($21)
	bne	$21,$0,$L1777
$L1775:
	lbu	$5,148($sp)
$L2236:
	.set	noreorder
	.set	nomacro
	beq	$21,$0,$L1778
	addiu	$23,$sp,148
	.set	macro
	.set	reorder

	lbu	$2,151($sp)
	lbu	$6,149($sp)
	lbu	$7,150($sp)
	lui	$4,%hi($LC51)
	addiu	$4,$4,%lo($LC51)
	andi	$5,$5,0x00ff
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	addiu	$4,$21,16
	sw	$0,460($21)
	move	$17,$0
	sw	$0,204($sp)
	sw	$0,240($sp)
	move	$20,$21
	sw	$4,244($sp)
$L2003:
	lbu	$2,10($20)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2237
	lw	$2,204($sp)
	.set	macro
	.set	reorder

	lw	$5,208($sp)
	addiu	$4,$20,4
	.set	noreorder
	.set	nomacro
	jal	memcmp
	li	$6,6			# 0x6
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1783
	move	$3,$0
	.set	macro
	.set	reorder

	lbu	$4,11($20)
	sra	$2,$4,$3
$L2239:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2238
	lw	$5,216($sp)
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	slt	$2,$3,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2239
	sra	$2,$4,$3
	.set	macro
	.set	reorder

	lw	$5,216($sp)
$L2238:
	addiu	$2,$3,1
	andi	$2,$2,0x00ff
	andi	$3,$5,0x7f
	.set	noreorder
	.set	nomacro
	beq	$3,$0,$L1790
	sw	$2,212($sp)
	.set	macro
	.set	reorder

	lbu	$4,11($20)
	.set	noreorder
	.set	nomacro
	beq	$4,$5,$L1790
	move	$3,$0
	.set	macro
	.set	reorder

	sra	$2,$4,$3
$L2241:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2240
	addiu	$2,$3,1
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	slt	$2,$3,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2241
	sra	$2,$4,$3
	.set	macro
	.set	reorder

	addiu	$2,$3,1
$L2240:
	andi	$2,$2,0x00ff
	sw	$2,212($sp)
	addu	$4,$2,$21
	lbu	$2,452($4)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1797
	addiu	$2,$2,-1
	.set	macro
	.set	reorder

	sll	$3,$2,24
	sra	$3,$3,24
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1797
	sb	$2,452($4)
	.set	macro
	.set	reorder

	lbu	$2,11($20)
	lbu	$3,455($21)
	move	$23,$0
	nor	$2,$0,$2
	and	$2,$2,$3
	sb	$2,455($21)
	lw	$6,148($sp)
	lui	$2,%hi(mcache)
	addiu	$3,$2,%lo(mcache)
	sw	$6,152($sp)
	lbu	$2,153($sp)
	lbu	$4,154($sp)
	lbu	$5,155($sp)
	sw	$6,156($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$19,$2,2
	addu	$3,$19,$3
	lw	$18,0($3)
$L2221:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L2242
	lbu	$2,150($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	beq	$2,$6,$L1800
	move	$23,$18
	.set	noreorder
	.set	nomacro
	j	$L2221
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1800:
	.set	noreorder
	.set	nomacro
	beq	$18,$0,$L2242
	lbu	$2,150($sp)
	.set	macro
	.set	reorder

	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2243
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L1806:
	lbu	$2,159($sp)
	lbu	$5,156($sp)
	lbu	$6,157($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,158($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1806
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L2243:
	addiu	$2,$3,%lo(mcache)
	addu	$3,$19,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1807
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1808
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1807:
	sw	$4,8($23)
$L1808:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

	lbu	$2,150($sp)
$L2242:
	lbu	$3,151($sp)
	lbu	$6,148($sp)
	lbu	$7,149($sp)
	lw	$5,212($sp)
	lui	$4,%hi($LC45)
	addiu	$4,$4,%lo($LC45)
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$3,20($sp)
	.set	macro
	.set	reorder

$L1797:
	lw	$4,216($sp)
	move	$3,$0
	sb	$4,11($20)
	lw	$5,216($sp)
	sra	$2,$5,$3
$L2245:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2244
	addiu	$2,$3,1
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	slt	$2,$3,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2245
	sra	$2,$5,$3
	.set	macro
	.set	reorder

	addiu	$2,$3,1
$L2244:
	andi	$2,$2,0x00ff
	sw	$2,212($sp)
	addu	$19,$2,$21
	lb	$2,452($19)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1816
	lw	$6,148($sp)
	.set	macro
	.set	reorder

	lui	$2,%hi(mcache)
	addiu	$3,$2,%lo(mcache)
	sw	$6,160($sp)
	lbu	$2,161($sp)
	lbu	$4,162($sp)
	lbu	$5,163($sp)
	sw	$6,164($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$23,$2,2
	addu	$3,$23,$3
	lw	$18,0($3)
	sw	$0,220($sp)
$L2222:
	beq	$18,$0,$L1828
	lw	$2,0($18)
	beq	$2,$6,$L1819
	sw	$18,220($sp)
	.set	noreorder
	.set	nomacro
	j	$L2222
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1819:
	beq	$18,$0,$L1828
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2246
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L1825:
	lbu	$2,167($sp)
	lbu	$5,164($sp)
	lbu	$6,165($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,166($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1825
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L2246:
	addiu	$2,$3,%lo(mcache)
	addu	$3,$23,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1826
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1827
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1826:
	lw	$5,220($sp)
	sw	$4,8($5)
$L1827:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L1828:
	lbu	$2,455($21)
	lw	$3,216($sp)
	lui	$4,%hi($LC46)
	addiu	$4,$4,%lo($LC46)
	or	$2,$3,$2
	sb	$2,455($21)
	lbu	$2,150($sp)
	lbu	$3,151($sp)
	lbu	$6,148($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lbu	$2,12($fp)
	lbu	$7,149($sp)
	lw	$5,212($sp)
	sw	$2,24($sp)
	lbu	$2,13($fp)
	sw	$2,28($sp)
	lbu	$2,14($fp)
	sw	$2,32($sp)
	lbu	$2,15($fp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,36($sp)
	.set	macro
	.set	reorder

$L1816:
	lbu	$2,452($19)
	addiu	$2,$2,1
	sb	$2,452($19)
$L1790:
	lw	$4,288($sp)
	lw	$2,20($4)
	sw	$0,56($20)
	lui	$4,%hi($LC52)
	sw	$2,12($20)
	lbu	$2,3($21)
	lbu	$5,0($21)
	lbu	$6,1($21)
	lbu	$7,2($21)
	sw	$2,16($sp)
	lw	$2,212($sp)
	addiu	$4,$4,%lo($LC52)
	sw	$2,20($sp)
	lw	$2,20($20)
	sw	$2,24($sp)
	lbu	$2,12($fp)
	sw	$2,28($sp)
	lbu	$2,13($fp)
	sw	$2,32($sp)
	lbu	$2,14($fp)
	sw	$2,36($sp)
	lbu	$2,15($fp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,40($sp)
	.set	macro
	.set	reorder

	lw	$2,20($20)
	li	$5,1			# 0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$5,$L1829
	lbu	$4,0($22)
	.set	macro
	.set	reorder

	andi	$3,$4,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$3,$5,$L1831
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1830
	li	$2,5			# 0x5
	.set	macro
	.set	reorder

$L1831:
	lui	$4,%hi($LC53)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	addiu	$17,$20,16
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1832
	sw	$2,16($20)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($20)
$L1832:
	lw	$5,16($20)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2247:
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$17
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2247
	sll	$2,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L1830:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L2248
	andi	$3,$4,0x00ff
	.set	macro
	.set	reorder

	lw	$2,16($20)
	slt	$2,$2,8
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2249
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	lui	$4,%hi($LC56)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC56)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	addiu	$18,$20,16
	sw	$0,224($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2159
	move	$23,$0
	.set	macro
	.set	reorder

	move	$17,$22
$L1849:
	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	lui	$4,%hi($LC57)
	addiu	$4,$4,%lo($LC57)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1844
	move	$19,$0
	.set	macro
	.set	reorder

	move	$16,$18
$L1847:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC58)
	addiu	$4,$4,%lo($LC58)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$3,8($16)
	lw	$2,8($17)
	addiu	$19,$19,1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1845
	addiu	$16,$16,4
	.set	macro
	.set	reorder

	lui	$4,%hi($LC59)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC59)
	.set	macro
	.set	reorder

	li	$3,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L1842
	sw	$3,224($sp)
	.set	macro
	.set	reorder

$L1845:
	lw	$2,0($18)
	slt	$2,$19,$2
	bne	$2,$0,$L1847
$L1844:
	lw	$4,224($sp)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L1842
	lui	$4,%hi($LC60)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC60)
	.set	macro
	.set	reorder

	lw	$3,0($18)
	lw	$4,8($17)
	sll	$2,$3,2
	addu	$2,$2,$18
	addiu	$3,$3,1
	sw	$4,8($2)
	sw	$3,0($18)
$L1842:
	lhu	$2,2($22)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1849
	addiu	$17,$17,4
	.set	macro
	.set	reorder

$L2159:
	lw	$2,0($18)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$23,$0
	.set	macro
	.set	reorder

	move	$16,$18
$L1853:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC61)
	addiu	$4,$4,%lo($LC61)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1853
	addiu	$16,$16,4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L2248:
	li	$2,2			# 0x2
$L2249:
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L1857
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1856
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

$L1857:
	lhu	$2,2($22)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1858
	lui	$4,%hi($LC53)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1859
	sw	$2,16($20)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($20)
$L1859:
	lw	$5,16($20)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2250:
	lw	$5,244($sp)
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$5
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$3,244($sp)
	lw	$2,0($3)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2250
	sll	$2,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L1858:
	sw	$0,20($20)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1866
	sw	$2,16($20)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($20)
$L1866:
	lw	$5,16($20)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2251:
	lw	$4,244($sp)
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$4
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$5,244($sp)
	lw	$2,0($5)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2251
	sll	$2,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L1856:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi($LC62)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC62)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	move	$23,$0
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2168
	move	$19,$0
	.set	macro
	.set	reorder

	move	$17,$22
$L1893:
	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	lui	$4,%hi($LC63)
	addiu	$4,$4,%lo($LC63)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$3,244($sp)
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1878
	move	$18,$0
	.set	macro
	.set	reorder

	move	$16,$3
$L1881:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC58)
	addiu	$4,$4,%lo($LC58)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$3,8($16)
	lw	$2,8($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2213
	addiu	$16,$16,4
	.set	macro
	.set	reorder

	lw	$4,244($sp)
	addiu	$18,$18,1
	lw	$2,0($4)
	slt	$2,$18,$2
	bne	$2,$0,$L1881
$L1878:
	.set	noreorder
	.set	nomacro
	bne	$23,$0,$L2252
	lw	$5,244($sp)
	.set	macro
	.set	reorder

	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	lui	$4,%hi($LC65)
	addiu	$4,$4,%lo($LC65)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2231
	lhu	$2,2($22)
	.set	macro
	.set	reorder

$L2213:
	lui	$4,%hi($LC64)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC64)
	.set	macro
	.set	reorder

	li	$23,1			# 0x1
	lw	$5,244($sp)
$L2252:
	li	$2,1			# 0x1
	lw	$3,0($5)
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1883
	addiu	$3,$3,-1
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1884
	sw	$0,8($5)
	.set	macro
	.set	reorder

$L1883:
	slt	$2,$18,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1884
	lw	$5,244($sp)
	.set	macro
	.set	reorder

	sll	$2,$18,2
	subu	$4,$3,$18
	addu	$3,$2,$5
$L1888:
	lw	$2,12($3)
	addiu	$4,$4,-1
	sw	$2,8($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L1888
	addiu	$3,$3,4
	.set	macro
	.set	reorder

$L1884:
	lw	$3,244($sp)
	move	$18,$0
	lw	$2,0($3)
	addiu	$2,$2,-1
	.set	noreorder
	.set	nomacro
	blez	$2,$L1876
	sw	$2,0($3)
	.set	macro
	.set	reorder

	lw	$16,244($sp)
$L1892:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC66)
	addiu	$4,$4,%lo($LC66)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$4,244($sp)
	addiu	$18,$18,1
	lw	$2,0($4)
	slt	$2,$18,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1892
	addiu	$16,$16,4
	.set	macro
	.set	reorder

$L1876:
	lhu	$2,2($22)
$L2231:
	addiu	$19,$19,1
	slt	$2,$19,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1893
	addiu	$17,$17,4
	.set	macro
	.set	reorder

$L2168:
	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

	lbu	$2,3($21)
	lbu	$5,0($21)
	lbu	$6,1($21)
	lbu	$7,2($21)
	sw	$2,16($sp)
	lbu	$2,12($fp)
	lui	$4,%hi($LC50)
	addiu	$4,$4,%lo($LC50)
	sw	$2,20($sp)
	lbu	$2,13($fp)
	sw	$2,24($sp)
	lbu	$2,14($fp)
	sw	$2,28($sp)
	lbu	$2,15($fp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	jal	local_bh_enable
	lw	$4,288($sp)
	lw	$5,0($21)
$L2228:
	jal	iptv_LeaveIgmpGroup
	.set	noreorder
	.set	nomacro
	j	$L2229
	lhu	$2,2($22)
	.set	macro
	.set	reorder

$L1829:
	andi	$3,$4,0x00ff
	.set	noreorder
	.set	nomacro
	beq	$3,$5,$L1901
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1900
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

$L1901:
	lui	$4,%hi($LC53)
	sw	$5,20($20)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	addiu	$17,$20,16
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1902
	sw	$2,16($20)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($20)
$L1902:
	lw	$5,16($20)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2253:
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$17
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2253
	sll	$2,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L1900:
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L1910
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1909
	li	$2,5			# 0x5
	.set	macro
	.set	reorder

$L1910:
	lhu	$2,2($22)
	addiu	$17,$20,16
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1911
	lui	$4,%hi($LC53)
	.set	macro
	.set	reorder

	sw	$5,20($20)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1912
	sw	$2,16($20)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($20)
$L1912:
	lw	$5,16($20)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2254:
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$17
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2254
	sll	$2,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L1911:
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1919
	sw	$2,16($20)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($20)
$L1919:
	lw	$5,16($20)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2255:
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$17
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2255
	sll	$2,$16,2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L1909:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1926
	li	$2,6			# 0x6
	.set	macro
	.set	reorder

	lui	$4,%hi($LC62)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC62)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	addiu	$18,$20,16
	sw	$0,228($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1899
	move	$23,$0
	.set	macro
	.set	reorder

	move	$17,$22
$L1946:
	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	lui	$4,%hi($LC63)
	addiu	$4,$4,%lo($LC63)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1931
	move	$19,$0
	.set	macro
	.set	reorder

	move	$16,$18
$L1934:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC58)
	addiu	$4,$4,%lo($LC58)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$3,8($16)
	lw	$2,8($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2214
	addiu	$16,$16,4
	.set	macro
	.set	reorder

	lw	$2,0($18)
	addiu	$19,$19,1
	slt	$2,$19,$2
	bne	$2,$0,$L1934
$L1931:
	lw	$5,228($sp)
	bne	$5,$0,$L1935
	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	lui	$4,%hi($LC65)
	addiu	$4,$4,%lo($LC65)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2232
	lhu	$2,2($22)
	.set	macro
	.set	reorder

$L2214:
	lui	$4,%hi($LC64)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC64)
	.set	macro
	.set	reorder

	li	$2,1			# 0x1
	sw	$2,228($sp)
$L1935:
	lw	$3,0($18)
	li	$2,1			# 0x1
	bne	$3,$2,$L1936
	.set	noreorder
	.set	nomacro
	j	$L1937
	sw	$0,8($18)
	.set	macro
	.set	reorder

$L1936:
	addiu	$3,$3,-1
	slt	$2,$19,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1937
	sll	$2,$19,2
	.set	macro
	.set	reorder

	subu	$4,$3,$19
	addu	$3,$2,$18
$L1941:
	lw	$2,12($3)
	addiu	$4,$4,-1
	sw	$2,8($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L1941
	addiu	$3,$3,4
	.set	macro
	.set	reorder

$L1937:
	lw	$2,0($18)
	move	$19,$0
	addiu	$2,$2,-1
	.set	noreorder
	.set	nomacro
	blez	$2,$L1929
	sw	$2,0($18)
	.set	macro
	.set	reorder

	move	$16,$18
$L1945:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC66)
	addiu	$4,$4,%lo($LC66)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	addiu	$19,$19,1
	slt	$2,$19,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1945
	addiu	$16,$16,4
	.set	macro
	.set	reorder

$L1929:
	lhu	$2,2($22)
$L2232:
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1946
	addiu	$17,$17,4
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

$L1926:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L2230
	lw	$6,148($sp)
	.set	macro
	.set	reorder

	lui	$4,%hi($LC62)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC62)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	addiu	$18,$20,16
	sw	$0,232($sp)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2184
	move	$23,$0
	.set	macro
	.set	reorder

	move	$17,$22
$L1969:
	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	lui	$4,%hi($LC63)
	addiu	$4,$4,%lo($LC63)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1954
	move	$19,$0
	.set	macro
	.set	reorder

	move	$16,$18
$L1957:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC58)
	addiu	$4,$4,%lo($LC58)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$3,8($16)
	lw	$2,8($17)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2215
	addiu	$16,$16,4
	.set	macro
	.set	reorder

	lw	$2,0($18)
	addiu	$19,$19,1
	slt	$2,$19,$2
	bne	$2,$0,$L1957
$L1954:
	lw	$3,232($sp)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L1958
	lui	$4,%hi($LC65)
	.set	macro
	.set	reorder

	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	addiu	$4,$4,%lo($LC65)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2233
	lhu	$2,2($22)
	.set	macro
	.set	reorder

$L2215:
	lui	$4,%hi($LC64)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC64)
	.set	macro
	.set	reorder

	li	$4,1			# 0x1
	sw	$4,232($sp)
$L1958:
	lw	$3,0($18)
	li	$2,1			# 0x1
	bne	$3,$2,$L1959
	.set	noreorder
	.set	nomacro
	j	$L1960
	sw	$0,8($18)
	.set	macro
	.set	reorder

$L1959:
	addiu	$3,$3,-1
	slt	$2,$19,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1960
	sll	$2,$19,2
	.set	macro
	.set	reorder

	subu	$4,$3,$19
	addu	$3,$2,$18
$L1964:
	lw	$2,12($3)
	addiu	$4,$4,-1
	sw	$2,8($3)
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L1964
	addiu	$3,$3,4
	.set	macro
	.set	reorder

$L1960:
	lw	$2,0($18)
	move	$19,$0
	addiu	$2,$2,-1
	.set	noreorder
	.set	nomacro
	blez	$2,$L1952
	sw	$2,0($18)
	.set	macro
	.set	reorder

	move	$16,$18
$L1968:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC66)
	addiu	$4,$4,%lo($LC66)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($18)
	addiu	$19,$19,1
	slt	$2,$19,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1968
	addiu	$16,$16,4
	.set	macro
	.set	reorder

$L1952:
	lhu	$2,2($22)
$L2233:
	addiu	$23,$23,1
	slt	$2,$23,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1969
	addiu	$17,$17,4
	.set	macro
	.set	reorder

$L2184:
	lui	$4,%hi($LC56)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC56)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	move	$23,$0
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2189
	move	$19,$0
	.set	macro
	.set	reorder

	move	$17,$22
$L1980:
	lbu	$5,8($17)
	lbu	$6,9($17)
	lbu	$7,10($17)
	lbu	$2,11($17)
	lui	$4,%hi($LC57)
	addiu	$4,$4,%lo($LC57)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$5,244($sp)
	lw	$2,0($5)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1975
	move	$18,$0
	.set	macro
	.set	reorder

	move	$16,$5
$L1978:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC58)
	addiu	$4,$4,%lo($LC58)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$3,8($16)
	lw	$2,8($17)
	addiu	$18,$18,1
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1976
	addiu	$16,$16,4
	.set	macro
	.set	reorder

	lui	$4,%hi($LC59)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC59)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1973
	li	$23,1			# 0x1
	.set	macro
	.set	reorder

$L1976:
	lw	$3,244($sp)
	lw	$2,0($3)
	slt	$2,$18,$2
	bne	$2,$0,$L1978
$L1975:
	.set	noreorder
	.set	nomacro
	bne	$23,$0,$L1973
	lui	$4,%hi($LC60)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC60)
	.set	macro
	.set	reorder

	lw	$4,244($sp)
	lw	$5,244($sp)
	lw	$3,0($4)
	lw	$4,8($17)
	sll	$2,$3,2
	addu	$2,$2,$5
	addiu	$3,$3,1
	sw	$4,8($2)
	sw	$3,0($5)
$L1973:
	lhu	$2,2($22)
	addiu	$19,$19,1
	slt	$2,$19,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1980
	addiu	$17,$17,4
	.set	macro
	.set	reorder

$L2189:
	lw	$3,244($sp)
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1899
	move	$19,$0
	.set	macro
	.set	reorder

	move	$16,$3
$L1984:
	lbu	$5,8($16)
	lbu	$6,9($16)
	lbu	$7,10($16)
	lbu	$2,11($16)
	lui	$4,%hi($LC61)
	addiu	$4,$4,%lo($LC61)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$4,244($sp)
	addiu	$19,$19,1
	lw	$2,0($4)
	slt	$2,$19,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L1984
	addiu	$16,$16,4
	.set	macro
	.set	reorder

$L1899:
	lw	$6,148($sp)
$L2230:
	lui	$5,%hi(mcache)
	addiu	$3,$5,%lo(mcache)
	sw	$6,168($sp)
	lbu	$2,169($sp)
	lbu	$4,170($sp)
	lbu	$5,171($sp)
	sw	$6,172($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$19,$2,2
	addu	$3,$19,$3
	lw	$18,0($3)
	move	$23,$0
$L2223:
	beq	$18,$0,$L1997
	lw	$2,0($18)
	beq	$2,$6,$L1988
	move	$23,$18
	.set	noreorder
	.set	nomacro
	j	$L2223
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L1988:
	beq	$18,$0,$L1997
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2256
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L1994:
	lbu	$2,175($sp)
	lbu	$5,172($sp)
	lbu	$6,173($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,174($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L1994
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L2256:
	addiu	$2,$3,%lo(mcache)
	addu	$3,$19,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L1995
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1996
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L1995:
	sw	$4,8($23)
$L1996:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L1997:
	lbu	$2,3($21)
	lw	$4,212($sp)
	lbu	$5,0($21)
	lbu	$6,1($21)
	lbu	$7,2($21)
	sw	$4,20($sp)
	sw	$2,16($sp)
	lw	$2,20($20)
	lui	$4,%hi($LC67)
	addiu	$4,$4,%lo($LC67)
	sw	$2,24($sp)
	lbu	$2,12($fp)
	li	$17,1			# 0x1
	move	$16,$0
	sw	$2,28($sp)
	lbu	$2,13($fp)
	sw	$2,32($sp)
	lbu	$2,14($fp)
	sw	$2,36($sp)
	lbu	$2,15($fp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,40($sp)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	.set	noreorder
	.set	nomacro
	blez	$2,$L1783
	lw	$5,240($sp)
	.set	macro
	.set	reorder

$L2257:
	lui	$4,%hi($LC55)
	addiu	$4,$4,%lo($LC55)
	sll	$2,$5,1
	addu	$2,$2,$16
	sll	$2,$2,2
	addu	$2,$2,$21
	lbu	$5,24($2)
	lbu	$6,25($2)
	lbu	$7,26($2)
	lbu	$2,27($2)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,16($20)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2257
	lw	$5,240($sp)
	.set	macro
	.set	reorder

$L1783:
	lw	$2,460($21)
	lw	$3,56($20)
	sltu	$2,$2,$3
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2237
	lw	$2,204($sp)
	.set	macro
	.set	reorder

	sw	$3,460($21)
	lw	$2,204($sp)
$L2237:
	lw	$3,244($sp)
	lw	$4,240($sp)
	addiu	$2,$2,1
	sw	$2,204($sp)
	addiu	$3,$3,56
	addiu	$4,$4,7
	slt	$2,$2,8
	sw	$3,244($sp)
	addiu	$20,$20,56
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2003
	sw	$4,240($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2004
	lbu	$2,151($sp)
	.set	macro
	.set	reorder

	j	$L2119
$L2216:
	lbu	$5,12($fp)
	lbu	$6,13($fp)
	lbu	$7,14($fp)
	lbu	$9,15($fp)
	lbu	$10,3($23)
	lbu	$3,1($23)
	lbu	$8,2($23)
	lbu	$2,148($sp)
	lui	$4,%hi($LC69)
	addiu	$4,$4,%lo($LC69)
	sw	$9,16($sp)
	sw	$2,20($sp)
	sw	$3,24($sp)
	sw	$8,28($sp)
	.set	noreorder
	.set	nomacro
	j	$L2227
	sw	$10,32($sp)
	.set	macro
	.set	reorder

$L2004:
	lbu	$5,148($sp)
	lbu	$6,149($sp)
	lbu	$7,150($sp)
	lui	$4,%hi($LC68)
	addiu	$4,$4,%lo($LC68)
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$23,$sp,148
	.set	macro
	.set	reorder

	move	$19,$21
	sw	$0,204($sp)
$L2041:
	lw	$5,204($sp)
	li	$2,6			# 0x6
	addiu	$5,$5,1
	sw	$5,204($sp)
	lbu	$3,0($22)
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2216
	slt	$4,$5,8
	.set	macro
	.set	reorder

	lbu	$2,10($19)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2009
	li	$2,1
	.set	macro
	.set	reorder

	sb	$2,10($19)
	lw	$2,216($sp)
	addiu	$4,$19,4
	li	$6,6			# 0x6
	sb	$2,11($19)
	lw	$3,288($sp)
	lw	$5,208($sp)
	addiu	$17,$19,16
	lw	$2,20($3)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,12($19)
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$0
	li	$6,40			# 0x28
	.set	noreorder
	.set	nomacro
	jal	memset
	sw	$0,56($19)
	.set	macro
	.set	reorder

	lui	$4,%hi($LC53)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L2012
	sw	$2,16($19)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($19)
$L2012:
	lw	$5,16($19)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($19)
	.set	noreorder
	.set	nomacro
	blez	$2,$L2198
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2258:
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$17
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2258
	sll	$2,$16,2
	.set	macro
	.set	reorder

$L2198:
	lbu	$3,0($22)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2019
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2019
	li	$2,5			# 0x5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2019
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2020
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	bne	$3,$2,$L2018
$L2020:
	lhu	$2,2($22)
	bne	$2,$0,$L2018
$L2019:
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L2021
	sw	$2,20($19)
	.set	macro
	.set	reorder

$L2018:
	sw	$0,20($19)
$L2021:
	move	$6,$0
	lw	$4,216($sp)
	sra	$2,$4,$6
$L2260:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2259
	lw	$7,148($sp)
	.set	macro
	.set	reorder

	addiu	$6,$6,1
	slt	$2,$6,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2260
	sra	$2,$4,$6
	.set	macro
	.set	reorder

	lw	$7,148($sp)
$L2259:
	lui	$5,%hi(mcache)
	addiu	$3,$5,%lo(mcache)
	sw	$7,176($sp)
	lbu	$2,177($sp)
	lbu	$4,178($sp)
	lbu	$5,179($sp)
	addiu	$6,$6,1
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$20,$2,2
	addu	$3,$20,$3
	andi	$6,$6,0x7
	lw	$18,0($3)
	sw	$6,212($sp)
	sw	$7,180($sp)
	sw	$0,236($sp)
$L2224:
	beq	$18,$0,$L2040
	lw	$2,0($18)
	beq	$2,$7,$L2031
	sw	$18,236($sp)
	.set	noreorder
	.set	nomacro
	j	$L2224
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L2031:
	beq	$18,$0,$L2040
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2261
	lui	$3,%hi(mcache)
	.set	macro
	.set	reorder

$L2037:
	lbu	$2,183($sp)
	lbu	$5,180($sp)
	lbu	$6,181($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,182($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L2037
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(mcache)
$L2261:
	addiu	$2,$3,%lo(mcache)
	addu	$3,$20,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L2038
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2039
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L2038:
	lw	$5,236($sp)
	sw	$4,8($5)
$L2039:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L2040:
	lbu	$2,455($21)
	lw	$3,216($sp)
	lui	$4,%hi($LC70)
	addiu	$4,$4,%lo($LC70)
	or	$2,$3,$2
	sb	$2,455($21)
	lbu	$5,3($23)
	lbu	$2,1($23)
	lbu	$3,2($23)
	lw	$6,20($19)
	sw	$5,24($sp)
	sw	$3,20($sp)
	sw	$2,16($sp)
	lbu	$2,12($fp)
	lbu	$7,148($sp)
	lw	$5,212($sp)
	sw	$2,28($sp)
	lbu	$2,13($fp)
	sw	$2,32($sp)
	lbu	$2,14($fp)
	sw	$2,36($sp)
	lbu	$2,15($fp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,40($sp)
	.set	macro
	.set	reorder

	lw	$4,212($sp)
	addu	$3,$4,$21
	lbu	$2,452($3)
	addiu	$2,$2,1
	.set	noreorder
	.set	nomacro
	j	$L2119
	sb	$2,452($3)
	.set	macro
	.set	reorder

$L2009:
	.set	noreorder
	.set	nomacro
	bne	$4,$0,$L2041
	addiu	$19,$19,56
	.set	macro
	.set	reorder

	j	$L2119
$L1778:
	lbu	$2,151($sp)
	lbu	$6,149($sp)
	lbu	$7,150($sp)
	lui	$4,%hi($LC71)
	addiu	$4,$4,%lo($LC71)
	andi	$5,$5,0x00ff
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lbu	$3,0($22)
	li	$2,6			# 0x6
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2081
	lui	$2,%hi(malloc_sizes+76)
	.set	macro
	.set	reorder

$L2046:
	lw	$4,%lo(malloc_sizes+76)($2)
	.set	noreorder
	.set	nomacro
	jal	kmem_cache_alloc
	li	$5,32			# 0x20
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2081
	move	$19,$2
	.set	macro
	.set	reorder

	move	$4,$2
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,468			# 0x1d4
	.set	macro
	.set	reorder

	lw	$3,148($sp)
	li	$2,1
	sb	$2,10($19)
	sw	$3,0($19)
	lw	$5,216($sp)
	addiu	$4,$19,4
	li	$6,6			# 0x6
	sb	$5,11($19)
	lw	$3,288($sp)
	lw	$5,208($sp)
	addiu	$17,$19,16
	lw	$2,20($3)
	sw	$0,56($19)
	.set	noreorder
	.set	nomacro
	jal	memcpy
	sw	$2,12($19)
	.set	macro
	.set	reorder

	move	$4,$17
	move	$5,$0
	.set	noreorder
	.set	nomacro
	jal	memset
	li	$6,40			# 0x28
	.set	macro
	.set	reorder

	lui	$4,%hi($LC53)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC53)
	.set	macro
	.set	reorder

	lhu	$2,2($22)
	slt	$3,$2,9
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L2082
	sw	$2,16($19)
	.set	macro
	.set	reorder

	li	$2,8			# 0x8
	sw	$2,16($19)
$L2082:
	lw	$5,16($19)
	lui	$4,%hi($LC54)
	.set	noreorder
	.set	nomacro
	jal	printk
	addiu	$4,$4,%lo($LC54)
	.set	macro
	.set	reorder

	lw	$2,16($19)
	.set	noreorder
	.set	nomacro
	blez	$2,$L2204
	move	$16,$0
	.set	macro
	.set	reorder

	sll	$2,$16,2
$L2262:
	addu	$3,$2,$22
	lw	$3,8($3)
	addu	$2,$2,$17
	lui	$4,%hi($LC55)
	sw	$3,8($2)
	lbu	$5,8($2)
	lbu	$6,9($2)
	lbu	$7,10($2)
	lbu	$2,11($2)
	addiu	$4,$4,%lo($LC55)
	addiu	$16,$16,1
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	lw	$2,0($17)
	slt	$2,$16,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2262
	sll	$2,$16,2
	.set	macro
	.set	reorder

$L2204:
	lbu	$3,0($22)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2089
	li	$2,3			# 0x3
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2089
	li	$2,5			# 0x5
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2089
	li	$2,2			# 0x2
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2090
	li	$2,4			# 0x4
	.set	macro
	.set	reorder

	bne	$3,$2,$L2088
$L2090:
	lhu	$2,2($22)
	bne	$2,$0,$L2088
$L2089:
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	j	$L2091
	sw	$2,20($19)
	.set	macro
	.set	reorder

$L2088:
	sw	$0,20($19)
$L2091:
	move	$4,$0
	lw	$5,216($sp)
	sra	$2,$5,$4
$L2264:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2263
	addiu	$2,$4,1
	.set	macro
	.set	reorder

	addiu	$4,$4,1
	slt	$2,$4,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2264
	sra	$2,$5,$4
	.set	macro
	.set	reorder

	addiu	$2,$4,1
$L2263:
	andi	$2,$2,0x7
	lbu	$3,455($19)
	sw	$2,212($sp)
	lw	$2,216($sp)
	addiu	$5,$20,%lo(iptvGroup)
	addu	$5,$18,$5
	or	$3,$2,$3
	lw	$2,212($sp)
	sb	$3,455($19)
	sw	$0,460($19)
	addu	$4,$2,$19
	li	$2,1
	sb	$2,452($4)
	sw	$0,464($19)
	lw	$2,0($5)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2099
	move	$21,$2
	.set	macro
	.set	reorder

$L2225:
	lw	$2,464($21)
	beq	$2,$0,$L2207
	.set	noreorder
	.set	nomacro
	j	$L2225
	lw	$21,464($21)
	.set	macro
	.set	reorder

$L2207:
	.set	noreorder
	.set	nomacro
	j	$L2104
	sw	$19,464($21)
	.set	macro
	.set	reorder

$L2099:
	sw	$19,0($5)
$L2104:
	lw	$6,148($sp)
	lui	$4,%hi(mcache)
	addiu	$3,$4,%lo(mcache)
	sw	$6,184($sp)
	lbu	$2,185($sp)
	lbu	$4,186($sp)
	lbu	$5,187($sp)
	sw	$6,188($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$20,$2,2
	addu	$3,$20,$3
	lw	$18,0($3)
	move	$21,$0
$L2226:
	beq	$18,$0,$L2116
	lw	$2,0($18)
	beq	$2,$6,$L2107
	move	$21,$18
	.set	noreorder
	.set	nomacro
	j	$L2226
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L2107:
	beq	$18,$0,$L2116
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2265
	lui	$5,%hi(mcache)
	.set	macro
	.set	reorder

$L2113:
	lbu	$2,191($sp)
	lbu	$5,188($sp)
	lbu	$6,189($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,190($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L2113
	move	$17,$16
	.set	macro
	.set	reorder

	lui	$5,%hi(mcache)
$L2265:
	addiu	$2,$5,%lo(mcache)
	addu	$3,$20,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L2114
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2115
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L2114:
	sw	$4,8($21)
$L2115:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L2116:
	lbu	$2,3($23)
	lw	$3,212($sp)
	lbu	$5,148($sp)
	sw	$2,16($sp)
	sw	$3,20($sp)
	lw	$2,20($19)
	lbu	$6,1($23)
	lbu	$7,2($23)
	sw	$2,24($sp)
	lbu	$2,12($fp)
	lui	$4,%hi($LC72)
	addiu	$4,$4,%lo($LC72)
	sw	$2,28($sp)
	lbu	$2,13($fp)
	sw	$2,32($sp)
	lbu	$2,14($fp)
	sw	$2,36($sp)
	lbu	$2,15($fp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,40($sp)
	.set	macro
	.set	reorder

	j	$L2119
$L2081:
	lbu	$2,3($23)
	lbu	$6,1($23)
	lbu	$5,148($sp)
	sw	$2,16($sp)
	lbu	$2,12($fp)
	lbu	$7,2($23)
	lui	$4,%hi($LC73)
	sw	$2,20($sp)
	lbu	$2,13($fp)
	addiu	$4,$4,%lo($LC73)
	sw	$2,24($sp)
	lbu	$2,14($fp)
	sw	$2,28($sp)
	lbu	$2,15($fp)
	sw	$2,32($sp)
$L2227:
	jal	printk
$L2119:
	jal	local_bh_enable
$L1739:
	lhu	$2,2($22)
$L2229:
	lw	$4,196($sp)
	lw	$5,200($sp)
	sll	$2,$2,2
	addiu	$4,$4,1
	addu	$2,$22,$2
	sltu	$3,$4,$5
	sw	$4,196($sp)
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L2220
	addiu	$22,$2,8
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L1732
	li	$5,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L1735:
	li	$2,18			# 0x12
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2124
	li	$2,22			# 0x16
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L2123
	li	$2,23			# 0x17
	.set	macro
	.set	reorder

$L2124:
	lw	$3,4($4)
	li	$2,536870912			# 0x20000000
	addu	$2,$3,$2
	sltu	$2,$2,256
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2219
	sw	$3,48($sp)
	.set	macro
	.set	reorder

	li	$2,-268500992			# 0xffffffffefff0000
	ori	$2,$2,0xfffa
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2219
	li	$2,1
	.set	macro
	.set	reorder

	sb	$2,0($7)
	li	$5,1			# 0x1
	li	$2,1
	.set	noreorder
	.set	nomacro
	j	$L1732
	sh	$2,0($6)
	.set	macro
	.set	reorder

$L2219:
	.set	noreorder
	.set	nomacro
	j	$L1732
	li	$5,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

$L2123:
	.set	noreorder
	.set	nomacro
	bne	$3,$2,$L1732
	li	$5,-1			# 0xffffffffffffffff
	.set	macro
	.set	reorder

	lw	$2,4($4)
	li	$3,2
	sb	$3,0($7)
	li	$3,1
	sh	$3,0($6)
	li	$5,1			# 0x1
	sw	$2,48($sp)
$L1732:
	li	$2,-1			# 0xffffffffffffffff
	.set	noreorder
	.set	nomacro
	beq	$5,$2,$L1730
	lhu	$2,112($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L1730
	move	$17,$0
	.set	macro
	.set	reorder

	addiu	$16,$sp,48
$L2140:
	addiu	$3,$sp,48
	addu	$2,$3,$17
	lb	$3,72($2)
	lui	$4,%hi($LC74)
	li	$2,1			# 0x1
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2137
	addiu	$4,$4,%lo($LC74)
	.set	macro
	.set	reorder

	lui	$4,%hi($LC50)
	li	$2,2			# 0x2
	.set	noreorder
	.set	nomacro
	beq	$3,$2,$L2138
	addiu	$4,$4,%lo($LC50)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2234
	lhu	$2,112($sp)
	.set	macro
	.set	reorder

$L2137:
	lbu	$2,3($16)
	lw	$3,192($sp)
	lbu	$5,0($16)
	lbu	$6,1($16)
	lbu	$7,2($16)
	sw	$2,16($sp)
	lbu	$2,12($3)
	sw	$2,20($sp)
	lbu	$2,13($3)
	sw	$2,24($sp)
	lbu	$2,14($3)
	sw	$2,28($sp)
	lbu	$2,15($3)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$4,288($sp)
	.set	noreorder
	.set	nomacro
	jal	iptv_AddIgmpGroup
	lw	$5,0($16)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2234
	lhu	$2,112($sp)
	.set	macro
	.set	reorder

$L2138:
	lbu	$2,3($16)
	lw	$3,192($sp)
	lbu	$5,0($16)
	lbu	$6,1($16)
	lbu	$7,2($16)
	sw	$2,16($sp)
	lbu	$2,12($3)
	sw	$2,20($sp)
	lbu	$2,13($3)
	sw	$2,24($sp)
	lbu	$2,14($3)
	sw	$2,28($sp)
	lbu	$2,15($3)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$4,288($sp)
	.set	noreorder
	.set	nomacro
	jal	iptv_LeaveIgmpGroup
	lw	$5,0($16)
	.set	macro
	.set	reorder

	lhu	$2,112($sp)
$L2234:
	addiu	$17,$17,1
	slt	$2,$17,$2
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2140
	addiu	$16,$16,4
	.set	macro
	.set	reorder

$L1730:
	lw	$31,284($sp)
	lw	$fp,280($sp)
	lw	$23,276($sp)
	lw	$22,272($sp)
	lw	$21,268($sp)
	lw	$20,264($sp)
	lw	$19,260($sp)
	lw	$18,256($sp)
	lw	$17,252($sp)
	lw	$16,248($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,288
	.set	macro
	.set	reorder

	.end	iptv_ProcessIgmpFromLan
	.align	2
	.globl	iptv_igmp_init
	.ent	iptv_igmp_init
	.type	iptv_igmp_init, @function
iptv_igmp_init:
	.frame	$sp,24,$31		# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	lui	$4,%hi(iptvGroup)
	addiu	$sp,$sp,-24
	addiu	$4,$4,%lo(iptvGroup)
	move	$5,$0
	sw	$31,16($sp)
	jal	memset
	li	$6,64			# 0x40

	lui	$2,%hi(jiffies)
	lw	$4,%lo(jiffies)($2)
	lui	$3,%hi(iptv_igmp_timer)
	lw	$31,16($sp)
	addiu	$2,$3,%lo(iptv_igmp_timer)
	sw	$4,4($2)
	addiu	$sp,$sp,24
	li	$2,1			# 0x1
	j	$31
	sw	$2,%lo(iptv_igmp_timer)($3)

	.set	macro
	.set	reorder
	.end	iptv_igmp_init
	.align	2
	.globl	iptv_igmp_shutdown
	.ent	iptv_igmp_shutdown
	.type	iptv_igmp_shutdown, @function
iptv_igmp_shutdown:
	.frame	$sp,88,$31		# vars= 8, regs= 10/0, args= 40, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-88
	lui	$2,%hi(iptv_igmp_timer)
	sw	$0,%lo(iptv_igmp_timer)($2)
	sw	$31,84($sp)
	sw	$fp,80($sp)
	sw	$23,76($sp)
	sw	$22,72($sp)
	sw	$21,68($sp)
	sw	$20,64($sp)
	sw	$19,60($sp)
	sw	$18,56($sp)
	sw	$17,52($sp)
	sw	$16,48($sp)
	lw	$2,20($28)
	addiu	$2,$2,256
	sw	$2,20($28)
	move	$21,$0
$L2294:
	lui	$3,%hi(iptvGroup)
	sll	$22,$21,2
	addiu	$2,$3,%lo(iptvGroup)
	addu	$2,$22,$2
	lw	$19,0($2)
	.set	noreorder
	.set	nomacro
	beq	$19,$0,$L2305
	addiu	$2,$3,%lo(iptvGroup)
	.set	macro
	.set	reorder

$L2293:
	lbu	$2,455($19)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2280
	lui	$fp,%hi(mcache)
	.set	macro
	.set	reorder

	lw	$6,0($19)
	addiu	$3,$fp,%lo(mcache)
	sw	$6,40($sp)
	lbu	$2,41($sp)
	lbu	$4,42($sp)
	lbu	$5,43($sp)
	sw	$6,44($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$20,$2,2
	addu	$3,$20,$3
	lw	$18,0($3)
	move	$23,$0
$L2304:
	beq	$18,$0,$L2280
	lw	$2,0($18)
	beq	$2,$6,$L2283
	move	$23,$18
	.set	noreorder
	.set	nomacro
	j	$L2304
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L2283:
	beq	$18,$0,$L2280
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2306
	addiu	$2,$fp,%lo(mcache)
	.set	macro
	.set	reorder

$L2289:
	lbu	$2,47($sp)
	lbu	$5,44($sp)
	lbu	$6,45($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,46($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L2289
	move	$17,$16
	.set	macro
	.set	reorder

	addiu	$2,$fp,%lo(mcache)
$L2306:
	addu	$3,$20,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L2290
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2291
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L2290:
	sw	$4,8($23)
$L2291:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L2280:
	lw	$16,464($19)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L2293
	move	$19,$16
	.set	macro
	.set	reorder

	lui	$3,%hi(iptvGroup)
	addiu	$2,$3,%lo(iptvGroup)
$L2305:
	addiu	$21,$21,1
	addu	$2,$22,$2
	slt	$3,$21,16
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L2294
	sw	$0,0($2)
	.set	macro
	.set	reorder

	jal	local_bh_enable
	lw	$31,84($sp)
	lw	$fp,80($sp)
	lw	$23,76($sp)
	lw	$22,72($sp)
	lw	$21,68($sp)
	lw	$20,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,88
	.set	macro
	.set	reorder

	.end	iptv_igmp_shutdown
	.local	iRow.0
	.comm	iRow.0,4,4
	.section	.rodata.str1.4
	.align	2
$LC75:
	.ascii	"iptv_igmp_gc: 1 : Remove server cache group [%d.%d.%d.%d"
	.ascii	"], port[%d]\n\000"
	.align	2
$LC76:
	.ascii	"iptv_igmp_gc: 2: Remove whole group [%d.%d.%d.%d]\n\000"
	.text
	.align	2
	.globl	iptv_igmp_gc
	.ent	iptv_igmp_gc
	.type	iptv_igmp_gc, @function
iptv_igmp_gc:
	.frame	$sp,104,$31		# vars= 24, regs= 10/0, args= 40, gp= 0
	.mask	0xc0ff0000,-4
	.fmask	0x00000000,0
	addiu	$sp,$sp,-104
	sw	$31,100($sp)
	sw	$fp,96($sp)
	sw	$23,92($sp)
	sw	$22,88($sp)
	sw	$21,84($sp)
	sw	$20,80($sp)
	sw	$19,76($sp)
	sw	$18,72($sp)
	sw	$17,68($sp)
	sw	$16,64($sp)
	lw	$2,20($28)
	sw	$0,56($sp)
	addiu	$2,$2,256
	sw	$2,20($28)
	lui	$3,%hi(iRow.0)
	lw	$2,%lo(iRow.0)($3)
	lui	$4,%hi(iptvGroup)
	addiu	$3,$4,%lo(iptvGroup)
	sll	$2,$2,2
	addu	$2,$2,$3
	lw	$19,0($2)
$L2378:
	beq	$19,$0,$L2365
	lw	$2,460($19)
	addiu	$2,$2,1
	sltu	$3,$2,1080
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L2315
	sw	$2,460($19)
	.set	macro
	.set	reorder

	sw	$0,460($19)
	move	$fp,$0
	move	$20,$19
$L2342:
	lbu	$2,10($20)
	beq	$2,$0,$L2318
	lw	$2,56($20)
	addiu	$2,$2,1
	sltu	$3,$2,1080
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L2320
	sw	$2,56($20)
	.set	macro
	.set	reorder

	lbu	$6,11($20)
	move	$3,$0
	sra	$2,$6,$3
$L2380:
	andi	$2,$2,0x1
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2379
	addiu	$2,$3,1
	.set	macro
	.set	reorder

	addiu	$3,$3,1
	slt	$2,$3,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2380
	sra	$2,$6,$3
	.set	macro
	.set	reorder

	addiu	$2,$3,1
$L2379:
	andi	$23,$2,0x7
	addu	$4,$23,$19
	lbu	$2,452($4)
	.set	noreorder
	.set	nomacro
	beq	$2,$0,$L2328
	addiu	$2,$2,-1
	.set	macro
	.set	reorder

	sll	$3,$2,24
	sra	$3,$3,24
	.set	noreorder
	.set	nomacro
	bne	$3,$0,$L2328
	sb	$2,452($4)
	.set	macro
	.set	reorder

	lbu	$3,455($19)
	lw	$5,0($19)
	nor	$2,$0,$6
	and	$2,$2,$3
	sb	$2,455($19)
	sw	$5,40($sp)
	lbu	$3,42($sp)
	lbu	$2,41($sp)
	lbu	$4,43($sp)
	move	$22,$0
	addu	$2,$2,$3
	addu	$2,$2,$4
	andi	$2,$2,0xf
	lui	$3,%hi(mcache)
	sll	$21,$2,2
	addiu	$2,$3,%lo(mcache)
	addu	$2,$21,$2
	lw	$18,0($2)
	sw	$5,44($sp)
$L2376:
	beq	$18,$0,$L2340
	lw	$2,0($18)
	beq	$2,$5,$L2331
	move	$22,$18
	.set	noreorder
	.set	nomacro
	j	$L2376
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L2331:
	beq	$18,$0,$L2340
	lw	$17,4($18)
	beq	$17,$0,$L2371
$L2337:
	lbu	$2,47($sp)
	lbu	$5,44($sp)
	lbu	$6,45($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,46($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L2337
	move	$17,$16
	.set	macro
	.set	reorder

$L2371:
	lui	$4,%hi(mcache)
	addiu	$2,$4,%lo(mcache)
	addu	$4,$21,$2
	lw	$2,0($4)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L2338
	lw	$3,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2339
	sw	$3,0($4)
	.set	macro
	.set	reorder

$L2338:
	sw	$3,8($22)
$L2339:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L2340:
	lbu	$5,0($19)
	lbu	$6,1($19)
	lbu	$7,2($19)
	lbu	$2,3($19)
	lui	$4,%hi($LC75)
	addiu	$4,$4,%lo($LC75)
	sw	$2,16($sp)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$23,20($sp)
	.set	macro
	.set	reorder

$L2328:
	sw	$0,56($20)
	sb	$0,10($20)
$L2320:
	lw	$2,460($19)
	lw	$3,56($20)
	sltu	$2,$2,$3
	beq	$2,$0,$L2318
	sw	$3,460($19)
$L2318:
	addiu	$fp,$fp,1
	slt	$2,$fp,8
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2342
	addiu	$20,$20,56
	.set	macro
	.set	reorder

	lbu	$2,455($19)
	.set	noreorder
	.set	nomacro
	bne	$2,$0,$L2343
	lw	$20,464($19)
	.set	macro
	.set	reorder

	lui	$3,%hi(iRow.0)
	lw	$2,%lo(iRow.0)($3)
	lui	$4,%hi(iptvGroup)
	addiu	$3,$4,%lo(iptvGroup)
	sll	$2,$2,2
	addu	$3,$2,$3
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$19,$2,$L2344
	lw	$2,56($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2345
	sw	$20,0($3)
	.set	macro
	.set	reorder

$L2344:
	sw	$20,464($2)
$L2345:
	lw	$6,0($19)
	lui	$23,%hi(mcache)
	addiu	$3,$23,%lo(mcache)
	sw	$6,48($sp)
	lbu	$2,49($sp)
	lbu	$4,50($sp)
	lbu	$5,51($sp)
	sw	$6,52($sp)
	addu	$2,$2,$4
	addu	$2,$2,$5
	andi	$2,$2,0xf
	sll	$21,$2,2
	addu	$3,$21,$3
	lw	$18,0($3)
	move	$22,$0
$L2377:
	beq	$18,$0,$L2357
	lw	$2,0($18)
	beq	$2,$6,$L2348
	move	$22,$18
	.set	noreorder
	.set	nomacro
	j	$L2377
	lw	$18,8($18)
	.set	macro
	.set	reorder

$L2348:
	beq	$18,$0,$L2357
	lw	$17,4($18)
	.set	noreorder
	.set	nomacro
	beq	$17,$0,$L2381
	addiu	$2,$23,%lo(mcache)
	.set	macro
	.set	reorder

$L2354:
	lbu	$2,55($sp)
	lbu	$5,52($sp)
	lbu	$6,53($sp)
	sw	$2,16($sp)
	lbu	$2,0($17)
	lbu	$7,54($sp)
	lui	$4,%hi($LC2)
	sw	$2,20($sp)
	lbu	$2,1($17)
	addiu	$4,$4,%lo($LC2)
	sw	$2,24($sp)
	lbu	$2,2($17)
	sw	$2,28($sp)
	lbu	$2,3($17)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,32($sp)
	.set	macro
	.set	reorder

	lw	$16,12($17)
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$17
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	bne	$16,$0,$L2354
	move	$17,$16
	.set	macro
	.set	reorder

	addiu	$2,$23,%lo(mcache)
$L2381:
	addu	$3,$21,$2
	lw	$2,0($3)
	.set	noreorder
	.set	nomacro
	bne	$18,$2,$L2355
	lw	$4,8($18)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2356
	sw	$4,0($3)
	.set	macro
	.set	reorder

$L2355:
	sw	$4,8($22)
$L2356:
	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$18
	.set	macro
	.set	reorder

$L2357:
	lbu	$5,0($19)
	lbu	$6,1($19)
	lbu	$7,2($19)
	lbu	$2,3($19)
	lui	$4,%hi($LC76)
	addiu	$4,$4,%lo($LC76)
	.set	noreorder
	.set	nomacro
	jal	printk
	sw	$2,16($sp)
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	jal	kfree
	move	$4,$19
	.set	macro
	.set	reorder

	.set	noreorder
	.set	nomacro
	j	$L2378
	move	$19,$20
	.set	macro
	.set	reorder

$L2343:
	sw	$19,56($sp)
	.set	noreorder
	.set	nomacro
	j	$L2378
	move	$19,$20
	.set	macro
	.set	reorder

$L2315:
	sw	$19,56($sp)
	.set	noreorder
	.set	nomacro
	j	$L2378
	lw	$19,464($19)
	.set	macro
	.set	reorder

$L2365:
	jal	local_bh_enable
	lui	$5,%hi(iRow.0)
	lw	$2,%lo(iRow.0)($5)
	lw	$31,100($sp)
	lw	$fp,96($sp)
	addiu	$4,$2,1
	slt	$3,$4,0
	addiu	$2,$2,16
	movz	$2,$4,$3
	sra	$2,$2,4
	lw	$23,92($sp)
	lw	$22,88($sp)
	lw	$21,84($sp)
	lw	$20,80($sp)
	lw	$19,76($sp)
	lw	$18,72($sp)
	lw	$17,68($sp)
	lw	$16,64($sp)
	sll	$2,$2,4
	subu	$4,$4,$2
	addiu	$sp,$sp,104
	.set	noreorder
	.set	nomacro
	j	$31
	sw	$4,%lo(iRow.0)($5)
	.set	macro
	.set	reorder

	.end	iptv_igmp_gc
	.globl	mgroup_lock
	.section	.bss
	.align	2
	.type	mgroup_lock, @object
	.size	mgroup_lock, 0
mgroup_lock:
	.space	1
	.ident	"GCC: (GNU) 3.4.4 (OpenWrt-2.0)"
