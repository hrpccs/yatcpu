struct screen {
	unsigned char row, col;
} scr;

void copy_line(int prev, int cur) {
	int *prev_vram_start = ((int *) (prev * 80 + 1024));
	int *cur_vram_start = ((int *) (cur * 80 + 1024));
	for (int i = 0; i < 20; ++i) {
		prev_vram_start[i] = cur_vram_start[i];
	}
}

void write_char(int row, int col, unsigned char ch) {
	unsigned char *vram = (unsigned char *) 1024;
	vram[row * 80 + col] = ch;
}

void move_to(int row, int col) {
	scr.row = row;
	scr.col = col;
}

void new_line() {
	scr.col = 0;
	if (scr.row == 29) {
		for (int i = 0; i < 29; ++i) {
			copy_line(i, i + 1);
		}
		int *vram = (int *) (29 * 80 + 1024);
		for (int i = 0; i < 20; ++i) {
			vram[i] = 0x20202020;
		}
	} else {
		++scr.row;
	}
}

void putch(unsigned char ch) {
	if (ch == '\n') {
		new_line();
	} else if (ch == '\r') { 
		scr.col = 0;
	} else {
		if (scr.col == 79) {
			new_line();
		}
		write_char(scr.row, scr.col, ch);
		++scr.col;
	}
}

void clear_screen() {
	scr.row = 0;
	scr.col = 0;
	int *vram = ((int *) 1024);
	for (int i = 0; i < 600; ++i) vram[i] = 0x20202020;
}

void print_hex(unsigned int counter) {
	putch('0'); putch('x');
	for (int i = 7; i >= 0; --i) {
		unsigned int num = (counter >> (i * 4)) & 0xF;
		if (num < 10) {
			putch('0' + num);
		} else {
			putch('A' + num - 10);
		}
	}
}

void putstr(const char *s) {
	while (*s) {
		putch(*(s++));
	}
}

int hc = 1;

void trap_handler(void *epc) {
	putstr("Hardware timer ticks! EPC = ");
	print_hex((unsigned int) epc);
	putstr(", Hardware counter = ");
	print_hex(hc++);
	putch('\n');
}

int main() {
	clear_screen();
	char mystr[] = "Software counter = ";
	putch(137);
	putstr("2021 Howard Lau\n");
	putstr("Hello, world!\n");
	*((int *) 0x4) = 0xDEADBEEF;
	unsigned int counter = 0;
	for (;;) {
		putstr("$ ");
		for (register int i = 0; i < 1000000; ++i);
		++counter;
		putstr(mystr);
		print_hex(counter);
		putch('\n');
	}
}
