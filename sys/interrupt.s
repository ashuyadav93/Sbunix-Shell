.global irq0

.extern irqHandler

irq0:
	cli
	movb 0x20, %al
	pushq %rax
	jmp irqStackSet

irqStackSet:
	pushq %rsp
	pushq %rbp
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
 	callq irqHandler
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	popq %rbp
	popq %rsp
	addq %rsp, 0x8
 	iretq
