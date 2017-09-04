#include <libc.h>

void exit(int status) {
	
	__asm__(
                "mov $1,%eax;"
                "mov $0,%ebx;"
                "int $0x80;"
	);
}

