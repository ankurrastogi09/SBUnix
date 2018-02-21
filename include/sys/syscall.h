#ifndef _SYSCALL1_H
#define _SYSCALL1_H

static inline uint64_t system_generic_1(uint64_t var0){
        uint64_t result;

        __asm__ __volatile__ (
                "movq %1, %%rax;"
                "syscall;"
                "movq %%rax, %0"
                : "=r" (result) 
                : "r" (var0)
                : "rax","rsp","r11");

        return result;
}

static inline uint64_t system_generic_2(uint64_t var0, uint64_t var1){
        uint64_t result;

        __asm__ __volatile__ (
                "movq %1, %%rax;"
                "movq %2, %%rdi;"
                "syscall;"
                "movq %%rax, %0"
                : "=r" (result) 
                : "r" (var0), "r" (var1)
                : "rax", "rdi");

        return result;
}

static inline uint64_t system_generic_3(uint64_t var0, uint64_t var1, uint64_t var2){
        uint64_t result;

        __asm__ __volatile__ (
                "movq %1, %%rax;"
                "movq %2, %%rdi;"
                "movq %3, %%rsi;"
                "syscall;"
                "movq %%rax, %0"
                : "=r" (result) 
                : "r" (var0), "r" (var1), "r" (var2)
                : "rax", "rdi", "rsi");

        return result;
}

static inline uint64_t system_generic_4(uint64_t var0, uint64_t var1, uint64_t var2, uint64_t var3){
        
        uint64_t result;

        __asm__ __volatile__ (
                "movq %1, %%rax;"
                "movq %2, %%rdi;"
                "movq %3, %%rsi;"
                "movq %4, %%rdx;"
                "syscall;"
                "movq %%rax, %0;"
                : "=r" (result) 
                : "r" (var0), "r" (var1), "r" (var2), "r" (var3)
                : "rax", "rdi", "rsi", "rdx");

        return result;
}

static inline uint64_t system_generic_5(uint64_t var0, uint64_t var1, uint64_t var2, uint64_t var3, uint64_t var4){
        
        uint64_t result;

        __asm__ __volatile__ (
                "movq %1, %%rax;"
                "movq %2, %%rdi;"
                "movq %3, %%rsi;"
                "movq %4, %%rdx;"
                "movq %5, %%r10;"
                "syscall;"
                "movq %%rax, %0;"
                : "=r" (result)
                : "r" (var0), "r" (var1), "r" (var2), "r" (var3), "r" (var4)
                : "rax", "rdi", "rsi", "rdx", "r10");

        return result;
}

static inline uint64_t system_generic_7(uint64_t var0, uint64_t var1, uint64_t var2, uint64_t var3, uint64_t var4, uint64_t var5, uint64_t var6){
        
        uint64_t result;

        __asm__ __volatile__ (
                "movq %1, %%rax;"
                "movq %2, %%rdi;"
                "movq %3, %%rsi;"
                "movq %4, %%rdx;"
                "movq %5, %%r10;"
                "movq %6, %%r8;"
                "movq %7, %%r9;"
                "syscall;"
                "movq %%rax, %0;"
                : "=r" (result)
                : "r" (var0), "r" (var1), "r" (var2), "r" (var3), "r" (var4), "r" (var5), "r" (var6)
                : "rax", "rdi", "rsi", "rdx", "r10", "r8", "r9");

        return result;
}

// static inline void system_w(const char* str, size_t len){
// 	__asm__ __volatile__ (
// 		"movq $1, %%rax;"
// 		"movq $1, %%rdi;"
// 		"movq %0, %%rsi;"
// 		"movq %1, %%rdx;"
// 		"syscall"
// 		:: "r" (str), "r" (len)
// 		: "rax","rdi", "rsi", "rdx");
// }

// static inline void system_w_g(uint64_t str, uint64_t len){
//         __asm__ __volatile__ (
//                 "movq $1, %%rax;"
//                 "movq $1, %%rdi;"
//                 "movq %0, %%rsi;"
//                 "movq %1, %%rdx;"
//                 "syscall"
//                 :: "r" (str), "r" (len)
//                 : "rax","rdi", "rsi", "rdx");
// }

static inline void system_e(int status){
	__asm__ __volatile__ (
		"movq $60, %%rax;"
		"movq $0, %%rdi;"
		"syscall"
		::
		: "rax","rdi");
}

#endif
