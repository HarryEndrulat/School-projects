#
# STUDENT: (Type your name here)
#

#
# AddressingLab (c) 2021 Christopher A. Bohn
#

##############################################################################
# This program will implement the Pleistocene Petting Zoo's Caesar Cipher and
# the cipher validation.
##############################################################################

	.text
	.file	"addressinglab.c"
	.globl	caesar_cipher           # -- Begin function caesar_cipher
	.p2align	4, 0x90
	.type	caesar_cipher,@function
caesar_cipher:                          # @caesar_cipher
	.cfi_startproc
# %bb.0:
	movb	(%rsi), %al
	movq	%rdi, %r8
	testb	%al, %al
	je	.LBB0_5
# %bb.1:
##### PLACE INSTRUCTION FOR TASK 1.1 ON NEXT LINE #####
	subl	$39, %edx
	incq	%rsi
	movq	%rdi, %r8
	.p2align	4, 0x90
.LBB0_2:                                # =>This Inner Loop Header: Depth=1
	movl	%eax, %ecx
	addb	$-65, %cl
	cmpb	$25, %cl
	ja	.LBB0_4
# %bb.3:                                #   in Loop: Header=BB0_2 Depth=1
##### PLACE INSTRUCTIONS FOR TASK 1.2 ON NEXT TWO LINES ##### #mov src, dest
	movsbl	%al, %eax
	addl	%edx, %eax
	cltq
	imulq	$1321528399, %rax, %r10 # imm = 0x4EC4EC4F
	movq	%r10, %r9
	shrq	$63, %r9
	shrq	$35, %r10
	addl	%r9d, %r10d
	leal	(%r10,%r10,4), %ecx
	leal	(%rcx,%rcx,4), %ecx
	addl	%r10d, %ecx
	subl	%ecx, %eax
	addb	$65, %al
.LBB0_4:                                #   in Loop: Header=BB0_2 Depth=1
##### PLACE INSTRUCTION FOR TASK 1.3 ON NEXT LINE #####
	movb	%al, (%r8)
##### PLACE INSTRUCTION FOR TASK 1.4 ON NEXT LINE #####
	movzbl	(%rsi), %eax
	addq	$1, %r8
	addq	$1, %rsi
	testb	%al, %al
	jne	.LBB0_2
.LBB0_5:
	movb	$0, (%r8)
	movq	%rdi, %rax
	retq
.Lfunc_end0:
	.size	caesar_cipher, .Lfunc_end0-caesar_cipher
	.cfi_endproc
                                        # -- End function
	.globl	sentence_to_uppercase   # -- Begin function sentence_to_uppercase
	.p2align	4, 0x90
	.type	sentence_to_uppercase,@function
sentence_to_uppercase:                  # @sentence_to_uppercase
	.cfi_startproc
# %bb.0:
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	pushq	%rax
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	%rsi, %r15
	movq	%rdi, %r14
	movq	%rsi, %rdi
	callq	strlen
	testq	%rax, %rax
	je	.LBB1_3
# %bb.1:
	movq	%rax, %r12
	xorl	%ebx, %ebx
	callq	__ctype_toupper_loc
	.p2align	4, 0x90
.LBB1_2:                                # =>This Inner Loop Header: Depth=1
	movq	(%rax), %rcx
##### PLACE INSTRUCTIONS FOR TASK 2.1 ON NEXT TWO LINES #####
	movsbq	(%r15, %rbx), %rdx
	movzbl	(%rcx, %rdx, 4), %ecx
##### PLACE INSTRUCTION FOR TASK 2.2 ON NEXT LINE #####
	movb	%cl, (%r14, %rbx)
	addq	$1, %rbx
	cmpq	%rbx, %r12
	jne	.LBB1_2
.LBB1_3:
	movq	%r14, %rax
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	sentence_to_uppercase, .Lfunc_end1-sentence_to_uppercase
	.cfi_endproc
                                        # -- End function
	.globl	validate_cipher         # -- Begin function validate_cipher
	.p2align	4, 0x90
	.type	validate_cipher,@function
validate_cipher:                        # @validate_cipher
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	pushq	%rax
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movq	%rdi, %rbx
	movq	(%rdi), %rdi
	callq	strlen
	movslq	16(%rbx), %rbp
	xorl	%r14d, %r14d
	movl	$0, %r15d
	cmpq	%rbp, %rax
	jne	.LBB2_2
# %bb.1:
	movq	8(%rbx), %rdi
	callq	strlen
	cmpq	%rbp, %rax
	sete	%r15b
.LBB2_2:
	movl	$256, %edi              # imm = 0x100
	callq	malloc
	movq	%rax, %rdi
	xorl	%edx, %edx
##### PLACE INSTRUCTIONS FOR TASK 3.1 ON NEXT TWO LINES #####
	movq	8(%rbx), %rsi
	subl	20(%rbx), %edx
	callq	caesar_cipher
	testb	%r15b, %r15b
	je	.LBB2_4
# %bb.3:
	movq	(%rbx), %rdi
	movslq	16(%rbx), %rdx
	movq	%rax, %rsi
	callq	strncmp
	testl	%eax, %eax
	sete	%r14b
.LBB2_4:
	movl	%r14d, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	validate_cipher, .Lfunc_end2-validate_cipher
	.cfi_endproc
                                        # -- End function

	.section	".note.GNU-stack","",@progbits
