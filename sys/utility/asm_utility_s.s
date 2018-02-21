.globl   getChildRip

getChildRip:
	lea (%rip), %rax
	mov %rsp, %rax
	mov %rax, 3392(%rdi)
	mov %rax, 3384(%rdi)
	mov %rbx, 3376(%rdi)
	mov %rcx, 3368(%rdi)
	mov %rdx, 3360(%rdi)
	mov %rsi, 3352(%rdi)
	mov %rdi, 3344(%rdi)
	mov %rbp, 3336(%rdi)
	mov %r8, 3328(%rdi)
	mov %r9, 3320(%rdi)
	mov %r10, 3312(%rdi)
	mov %r11, 3304(%rdi)
	mov %r12, 3296(%rdi)
	mov %r13, 3288(%rdi)
	mov %r14, 3280(%rdi)
	mov %r15, 3272(%rdi)
	ret