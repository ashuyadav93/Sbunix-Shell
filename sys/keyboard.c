#include <sys/interrupt.h>
#include <sys/kprintf.h>

//1st row: glyph
//2nd row: key press(0) or release(1)
uint8_t scancodes[256][2];

void initKeyboard() {

	initScanCodeTable();
}

void addScanCode(uint8_t pos, uint8_t val, uint8_t state) {
	
	scancodes[pos][0] = val;
	scancodes[pos][1] = state;
}

void initScanCodeTable() {
	uint8_t i = 0;
	for(i = 0; i < 255; i++)
		scancodes[i][1] = 1;
	//add numbers
	for(i = 2; i <= 11; i++) {
		scancodes[i][0] = (i-1)%10 + 48;
		scancodes[i][1] = 0;
	}
	for(i = 0x82; i <= 0x8B; i++) {
                scancodes[i][0] = (i-0x81)%10 + 48;
                scancodes[i][1] = 1;
        }
	
	//add alphabets
	i = 0x10;
	addScanCode(i++, 'q', 0);
	addScanCode(i++, 'w', 0);
	addScanCode(i++, 'e', 0);
	addScanCode(i++, 'r', 0);
	addScanCode(i++, 't', 0);
	addScanCode(i++, 'y', 0);
	addScanCode(i++, 'u', 0);
	addScanCode(i++, 'i', 0);
	addScanCode(i++, 'o', 0);
	addScanCode(i++, 'p', 0);
	i = 0x1E;
	addScanCode(i++, 'a', 0);
	addScanCode(i++, 's', 0);
	addScanCode(i++, 'd', 0);
	addScanCode(i++, 'f', 0);
	addScanCode(i++, 'g', 0);
	addScanCode(i++, 'h', 0);
	addScanCode(i++, 'j', 0);
	addScanCode(i++, 'k', 0);
	addScanCode(i++, 'l', 0);
	i = 0x2C;
	addScanCode(i++, 'z', 0);
	addScanCode(i++, 'x', 0);
	addScanCode(i++, 'c', 0);
	addScanCode(i++, 'v', 0);
	addScanCode(i++, 'b', 0);
	addScanCode(i++, 'n', 0);
	addScanCode(i++, 'm', 0);
	i = 0x90;
        addScanCode(i++, 'q', 1);
        addScanCode(i++, 'w', 1);
        addScanCode(i++, 'e', 1);
        addScanCode(i++, 'r', 1);
        addScanCode(i++, 't', 1);
        addScanCode(i++, 'y', 1);
        addScanCode(i++, 'u', 1);
        addScanCode(i++, 'i', 1);
        addScanCode(i++, 'o', 1);
        addScanCode(i++, 'p', 1);
        i = 0x9E;
        addScanCode(i++, 'a', 1);
        addScanCode(i++, 's', 1);
        addScanCode(i++, 'd', 1);
        addScanCode(i++, 'f', 1);
        addScanCode(i++, 'g', 1);
        addScanCode(i++, 'h', 1);
        addScanCode(i++, 'j', 1);
        addScanCode(i++, 'k', 1);
        addScanCode(i++, 'l', 1);
        i = 0xAC;
        addScanCode(i++, 'z', 1);
        addScanCode(i++, 'x', 1);
        addScanCode(i++, 'c', 1);
        addScanCode(i++, 'v', 1);
        addScanCode(i++, 'b', 1);
        addScanCode(i++, 'n', 1);
        addScanCode(i++, 'm', 1);
	
	//Shift anc Ctrl
	addScanCode(0x2A, 128, 0);
	addScanCode(0x36, 128, 0);
	addScanCode(0xAA, 128, 1);
        addScanCode(0xB6, 128, 1);

        addScanCode(0xE0, 129, 0);
        addScanCode(0x1D, 129, 0);
	addScanCode(0xE0, 129, 1);
        addScanCode(0x9D, 129, 1);

	//TODO
}

uint8_t special_char = 0;

void keyboardImpl() {
	
	uint8_t input = inIO(0x60);
	if(scancodes[input][1]) {
		return;
	}
	uint8_t c = scancodes[input][0];
	if(c < 127) {
		if(!special_char) {
			char s[] = {(char)c, '\0'};
			kprintf("Input character: %s\n", s);
			return;
		} else {
			if(special_char == 128) {
				if(c >= 97 && c <= 122) {
					c -= 32;
					char s[] = {(char)c, '\0'};
					kprintf("Input character: %s\n", s);
				}
			} else if(special_char == 129) {
				if(c >= 97 && c <= 122) {
                                        char s[] = {'^', (char)(c-32), '\0'};
                                        kprintf("Input character: %s\n", s);
                                }
			}
			special_char = 0;
		}
	}
	if(scancodes[input][0] > 127) {
		special_char = scancodes[input][0];
	}
}
