.globl userContextSwitch

userContextSwitch:
    cli
    mov 4128(%rdi), %rsp //Assigning passed user stack to rsp
    mov %rsp, %rax //now setting up the stack as required by user stack
    push $0x23
    push %rax
    pushf
    pop %rax
    or $0x200, %rax
    push %rax
    push $0x2B
    push 4112(%rdi)
    iretq