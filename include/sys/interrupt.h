#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include <sys/defs.h>

struct idt_table {
	uint16_t offset1;
	uint16_t offset2;
	uint32_t offset3;
 	uint16_t sel;		// The kernel segment
 	uint32_t always0;	// This value is always ... 0!
	uint8_t flags;
	uint8_t ist;		// bits 0 .. 2 hold interrupt stack table offset, rest of bits zero
}__attribute__((packed));


struct idt_ptr {
	uint16_t limit;
	uint64_t baseptr;
}__attribute__((packed));


struct regs {
	uint64_t gs, fs, es, ds;
	uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rip, cs, eflags, orig_rax, ss;
}__attribute__((packed));


void loadIdt(struct idt_ptr idtptr);

void setVector(int pos, uint64_t baseptr, uint16_t sel, uint8_t flags);

void initIdt();


extern void irq0();

void irqHandler(struct regs *r);


void outIO(uint16_t port, uint8_t value);

uint8_t inIO(uint16_t port);

#endif
