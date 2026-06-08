	.file	"t02_otimizacao.c"
	.text
	.p2align 4
	.globl	soma_simples
	.type	soma_simples, @function
soma_simples:
.LFB23:
	.cfi_startproc
	endbr64
	ret
	.cfi_endproc
.LFE23:
	.size	soma_simples, .-soma_simples
	.p2align 4
	.globl	soma_alternada
	.type	soma_alternada, @function
soma_alternada:
.LFB29:
	.cfi_startproc
	endbr64
	ret
	.cfi_endproc
.LFE29:
	.size	soma_alternada, .-soma_alternada
	.p2align 4
	.globl	test_func
	.type	test_func, @function
test_func:
.LFB25:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$72, %rsp
	.cfi_def_cfa_offset 128
	movq	%fs:40, %rbx
	movq	%rbx, 56(%rsp)
	movl	%ecx, %ebx
	testl	%ecx, %ecx
	jle	.L8
	leaq	32(%rsp), %rax
	movl	%edi, %r12d
	movq	%rsi, %rbp
	movq	%rdx, %r13
	movq	%rax, 8(%rsp)
	xorl	%r14d, %r14d
	pxor	%xmm0, %xmm0
	.p2align 4
	.p2align 3
.L6:
	leaq	16(%rsp), %rsi
	movl	$1, %edi
	movsd	%xmm0, (%rsp)
	addl	$1, %r14d
	call	clock_gettime@PLT
	movl	%r12d, %esi
	movq	%r13, %rdi
	call	*%rbp
	movq	8(%rsp), %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	32(%rsp), %rax
	pxor	%xmm1, %xmm1
	subq	16(%rsp), %rax
	cvtsi2sdq	%rax, %xmm1
	pxor	%xmm2, %xmm2
	movq	40(%rsp), %rax
	subq	24(%rsp), %rax
	mulsd	.LC1(%rip), %xmm1
	cvtsi2sdq	%rax, %xmm2
	movsd	(%rsp), %xmm0
	divsd	.LC2(%rip), %xmm2
	addsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	cmpl	%r14d, %ebx
	jne	.L6
.L5:
	pxor	%xmm1, %xmm1
	cvtsi2sdl	%ebx, %xmm1
	divsd	%xmm1, %xmm0
	movq	56(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L11
	addq	$72, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L8:
	.cfi_restore_state
	pxor	%xmm0, %xmm0
	jmp	.L5
.L11:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE25:
	.size	test_func, .-test_func
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC7:
	.string	"Vetor de tamanho %d levou %f milissegundos em m\303\251dia (%d execu\303\247\303\265es) usando soma simples.\n"
	.align 8
.LC8:
	.string	"Vetor de tamanho %d levou %f milissegundos em m\303\251dia (%d execu\303\247\303\265es) usando soma alternada.\n\n"
	.text
	.p2align 4
	.globl	compare_time
	.type	compare_time, @function
compare_time:
.LFB26:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$64, %rsp
	.cfi_offset 14, -24
	.cfi_offset 13, -32
	.cfi_offset 12, -40
	.cfi_offset 3, -48
	movq	%fs:40, %r13
	movq	%r13, -40(%rbp)
	movl	%edi, %r13d
	movq	%rsp, %rdx
	movslq	%r13d, %rax
	leaq	15(,%rax,4), %rax
	movq	%rax, %rcx
	andq	$-4096, %rcx
	subq	%rcx, %rdx
	cmpq	%rdx, %rsp
	je	.L14
.L29:
	subq	$4096, %rsp
	orq	$0, 4088(%rsp)
	cmpq	%rdx, %rsp
	jne	.L29
.L14:
	andl	$4080, %eax
	subq	%rax, %rsp
	testq	%rax, %rax
	jne	.L30
.L15:
	testl	%r13d, %r13d
	jle	.L16
	leal	-1(%r13), %eax
	cmpl	$2, %eax
	jbe	.L16
	movl	%r13d, %edx
	pcmpeqd	%xmm3, %xmm3
	movl	$4, %ecx
	movq	%rsp, %rax
	shrl	$2, %edx
	psrld	$31, %xmm3
	movd	%ecx, %xmm2
	movdqa	.LC3(%rip), %xmm1
	salq	$4, %rdx
	pshufd	$0, %xmm2, %xmm2
	addq	%rsp, %rdx
	.p2align 5
	.p2align 4
	.p2align 3
.L18:
	movdqa	%xmm1, %xmm0
	addq	$16, %rax
	paddd	%xmm2, %xmm1
	pslld	$1, %xmm0
	paddd	%xmm3, %xmm0
	movups	%xmm0, -16(%rax)
	cmpq	%rdx, %rax
	jne	.L18
.L16:
	movl	$10, %r14d
	pxor	%xmm0, %xmm0
	.p2align 4
	.p2align 3
.L19:
	leaq	-80(%rbp), %rsi
	movl	$1, %edi
	movsd	%xmm0, -88(%rbp)
	call	clock_gettime@PLT
	leaq	-64(%rbp), %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	-64(%rbp), %rax
	pxor	%xmm1, %xmm1
	subq	-80(%rbp), %rax
	cvtsi2sdq	%rax, %xmm1
	pxor	%xmm2, %xmm2
	movq	-56(%rbp), %rax
	subq	-72(%rbp), %rax
	mulsd	.LC1(%rip), %xmm1
	cvtsi2sdq	%rax, %xmm2
	movsd	-88(%rbp), %xmm0
	divsd	.LC2(%rip), %xmm2
	addsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	subl	$1, %r14d
	jne	.L19
	movl	$10, %ecx
	movl	%r13d, %edx
	movl	$2, %edi
	movl	$1, %eax
	leaq	.LC7(%rip), %rsi
	divsd	.LC6(%rip), %xmm0
	movl	$10, %r14d
	call	__printf_chk@PLT
	pxor	%xmm0, %xmm0
	.p2align 4
	.p2align 3
.L20:
	leaq	-80(%rbp), %rsi
	movl	$1, %edi
	movsd	%xmm0, -88(%rbp)
	call	clock_gettime@PLT
	leaq	-64(%rbp), %rsi
	movl	$1, %edi
	call	clock_gettime@PLT
	movq	-64(%rbp), %rax
	pxor	%xmm1, %xmm1
	subq	-80(%rbp), %rax
	cvtsi2sdq	%rax, %xmm1
	pxor	%xmm2, %xmm2
	movq	-56(%rbp), %rax
	subq	-72(%rbp), %rax
	mulsd	.LC1(%rip), %xmm1
	cvtsi2sdq	%rax, %xmm2
	movsd	-88(%rbp), %xmm0
	divsd	.LC2(%rip), %xmm2
	addsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	subl	$1, %r14d
	jne	.L20
	movl	$10, %ecx
	movl	%r13d, %edx
	movl	$2, %edi
	movl	$1, %eax
	leaq	.LC8(%rip), %rsi
	divsd	.LC6(%rip), %xmm0
	call	__printf_chk@PLT
	movq	-40(%rbp), %rax
	subq	%fs:40, %rax
	jne	.L31
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L30:
	.cfi_restore_state
	orq	$0, -8(%rsp,%rax)
	jmp	.L15
.L31:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE26:
	.size	compare_time, .-compare_time
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB27:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$1000, %edi
	call	compare_time
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE27:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1083129856
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC3:
	.long	0
	.long	1
	.long	2
	.long	3
	.section	.rodata.cst8
	.align 8
.LC6:
	.long	0
	.long	1076101120
	.ident	"GCC: (Ubuntu 15.2.0-16ubuntu1) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
