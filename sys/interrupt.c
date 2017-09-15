#include <sys/interrupt.h>
#include <sys/defs.h>
#include <sys/kprintf.h>

struct idt_table idtTable[256];

struct idt_ptr idtptr;

uint64_t ticks = 0;
uint64_t seconds = 0;


void setVector(int pos, uint64_t baseptr, uint16_t sel, uint8_t flags) {
	
	idtTable[pos].offset1 = (baseptr & 0x000000000000FFFF);
	idtTable[pos].offset2 = ((baseptr >> 16) & 0x000000000000FFFF);
	idtTable[pos].offset3 = ((baseptr >> 16) & 0x00000000FFFFFFFF);
	idtTable[pos].sel = sel;
	idtTable[pos].always0 = 0;
	idtTable[pos].flags = flags;
	idtTable[pos].ist = 0;
}

void loadIdt(struct idt_ptr idtptr) {
	__asm__
	(
		"lidt %0"
		:
		:"m"(idtptr)
	);
}

void initIdt() {

	idtptr.limit = (sizeof(struct idt_table) * 256) - 1;
	idtptr.baseptr = (uint64_t)&idtTable;

	for(int i = 0; i < 256; i++) {
		setVector(i, (uint64_t)irq0, 0x08, 0x8E);
	}

	/*outIO(0x20, 0x11);
	outIO(0xA0, 0x11);
	outIO(0x21, 0x20);
	outIO(0xA1, 0x28);
	outIO(0x21, 0x04);
	outIO(0xA1, 0x02);
	outIO(0x21, 0x01);
	outIO(0xA1, 0x01);
	outIO(0x21, 0x0);
	outIO(0xA1, 0x0);*/
	
	setVector(32, (uint64_t)irq0, 0x08, 0x8E);
	
	loadIdt(idtptr);

	__asm__("sti");

	__asm__("int $32");
}


void irqHandler(struct regs *r) {

	kprintf("in handler");	
	ticks++;
	if(ticks % 18 == 0) {
		kprintf("1 sec ");
	}

	outIO(0x20, 0x20);
}


void outIO(uint16_t port, uint8_t value) {	
	__asm__ volatile
	(
		"outb %1, %0 \n\t"
		:
		:"Nd"(port) , "a"(value)
	);
}

uint8_t inIO(uint16_t port) {
        uint8_t ret;
        __asm__ volatile
        (
                "inb %1, %0 \n\t"
                :"=a"(ret)
                :"Nd"(port)
        );
	return ret;
}
