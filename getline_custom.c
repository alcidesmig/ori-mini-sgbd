#include "getline_custom.h"

struct termios TERM_CONFIG;
struct termios TERM_CONFIG_OLD;
int INSERT_MODE = 0;
int TERM_READY = 0;

void move_cursor_rigth(int i) {
	if (i) printf("\033[%dC", i);
}

void move_cursor_left(int i) {
	if (i) printf("\033[%dD", i);
}

void term_init() {
	if (tcgetattr(STDIN_FILENO, &TERM_CONFIG) < 0) {
		printf("Falha ao obter as configurações\n");
		exit(1);
	}

	TERM_CONFIG_OLD = TERM_CONFIG;

	TERM_CONFIG.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

	TERM_CONFIG.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

	TERM_CONFIG.c_cflag &= ~(CSIZE | PARENB);
	TERM_CONFIG.c_cflag |= CS8;

	TERM_CONFIG.c_cc[VMIN]  = 1;
	TERM_CONFIG.c_cc[VTIME] = 0;

	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &TERM_CONFIG) < 0) {
		printf("Erro ao aplicar configuração\n");
		exit(1);
	}

	TERM_READY = 1;
}

void term_close() {
	if (TERM_READY) {
		if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &TERM_CONFIG_OLD) < 0) {
			printf("Erro ao aplicar configuração\n");
			exit(1);
		}

		TERM_READY = 0;
	}
}

void exit_term_close(int n) {
	term_close();
	exit(n);
}

int is_ASCII_char(char c) {
	return !(c & 0b10000000);
}

int is_marker_char(char c) {
	return (c & 0b11000000) == 0b11000000;
}

int is_payload_char(char c) {
	return (c & 0b11000000) == 0b10000000;
}

int move_left(char *line, int pos) {
	if (pos > 0) {
		pos--;

		if (is_ASCII_char(line[pos])) {
			return pos;
		} else if (is_marker_char(line[pos])) {
			return pos;
		} else if (is_payload_char(line[pos])) {
			return move_left(line, pos);
		}
	}

	return pos;
}

int move_right(char *line, int pos) {
	if (line[pos] != '\0') {
		pos++;

		if (is_ASCII_char(line[pos])) {
			return pos;
		} else if (is_marker_char(line[pos])) {
			return pos;
		} else if (is_payload_char(line[pos])) {
			return move_right(line, pos);
		}
	}

	return pos;
}

void put_multichar(char *line, char *sub, int size, int ssize, int place) {
	strncpy(&line[place+ssize], &line[place], size-place);
	strncpy(&line[place], sub, ssize);
	line[size+ssize] = '\0';
}

int get_multichar_length(char c) {
	int csize = 0;

	if (c == '\0') {
		return csize;
	} else if (is_ASCII_char(c)) {
		csize = 1;
	} else if (is_marker_char(c)) {
		if ((c & 0b11111111) == 0b11111111) {
			csize = 8;
		} else if ((c & 0b11111110) == 0b11111110) {
			csize = 7;
		} else if ((c & 0b11111100) == 0b11111100) {
			csize = 6;
		} else if ((c & 0b11111000) == 0b11111000) {
			csize = 5;
		} else if ((c & 0b11110000) == 0b11110000) {
			csize = 4;
		} else if ((c & 0b11100000) == 0b11100000) {
			csize = 3;
		} else if ((c & 0b11000000) == 0b11000000) {
			csize = 2;
		}
	}
		
	return csize;
}

int remove_multichar(char *line, int size, int place) {
	int csize = get_multichar_length(line[place]);

	strncpy(&line[place], &line[place+csize], size-place-csize+1);
	line[size-csize+1] = '\0';

	return csize;
}

int replace_multichar(char *line, char *sub, int size, int ssize, int place) {
	int csize = get_multichar_length(line[place]);

	strncpy(&line[place+ssize], &line[place+csize], size-place-csize+1);
	strncpy(&line[place], sub, ssize);
	line[size - csize + ssize + 1] = '\0';

	return csize;
}

int getline_custom(char **lineptr, ssize_t *n) {
	char buffer[8];
	int bsize;

	char line[256] = "\0";
	int pos = 0;
	int size = 0;

	int cursor = 0;
	int scursor = 0;

	if (!TERM_READY) {
		printf("Terminal não foi configurado.\n");
		exit(0);
	}

	do {
		bsize = read(STDIN_FILENO, buffer, sizeof(buffer));

		strncpy(buffer, buffer, bsize);
		buffer[bsize] = '\0';

		if (buffer[0] == 0x0) { // NUL
		// } else if (buffer[0] == 0x1) { // SOH
		// } else if (buffer[0] == 0x2) { // STX
		} else if (buffer[0] == 0x3) { // ETX
			exit_term_close(0);
		} else if (buffer[0] == 0x4) { // EOT
			return EOF;
		// } else if (buffer[0] == 0x5) { // ENQ
		// } else if (buffer[0] == 0x6) { // ACK
		// } else if (buffer[0] == 0x7) { // BEL
		// } else if (buffer[0] == 0x8) { // BS
		} else if (buffer[0] == 0x9) { // TAB
			printf("\t");
		// } else if (buffer[0] == 0xa) { // LF
		// } else if (buffer[0] == 0xb) { // VT
		// } else if (buffer[0] == 0xc) { // FF
		} else if (buffer[0] == 0xd) { // CR
			printf("\n");
			
			size++;

			*lineptr = realloc(*lineptr, size*sizeof(char));
			
			*n = size;
			
			strncpy(*lineptr, line, size);

			return size;
		// } else if (buffer[0] == 0xe) { // SO
		// } else if (buffer[0] == 0xf) { // SI
		// } else if (buffer[0] == 0x10) { // DLE
		// } else if (buffer[0] == 0x11) { // DC1
		// } else if (buffer[0] == 0x12) { // DC2
		// } else if (buffer[0] == 0x13) { // DC3
		// } else if (buffer[0] == 0x14) { // DC4
		// } else if (buffer[0] == 0x15) { // NAK
		// } else if (buffer[0] == 0x16) { // SYN
		// } else if (buffer[0] == 0x17) { // ETB
		// } else if (buffer[0] == 0x18) { // CAN
		// } else if (buffer[0] == 0x19) { // EM
		// } else if (buffer[0] == 0x1a) { // SUB
		} else if (buffer[0] == 0x1b) { // ESQ
			if (buffer[1] == 0x5b) {
				if(buffer[2] == 0x31) {
					// if(buffer[3] == 0x35) {
					// 	if(buffer[4] == 0x7e) { // F5
					// 	}
					// } else if(buffer[3] == 0x36) {
					// 	if(buffer[4] == 0x7e) { // F5
					// 	}
					// } else if(buffer[3] == 0x37) {
					// 	if(buffer[4] == 0x7e) { // F6
					// 	}
					// }  else if(buffer[3] == 0x38) {
					// 	if(buffer[4] == 0x7e) { // F7
					// 	}
					// } else if(buffer[3] == 0x39) {
					// 	if(buffer[4] == 0x7e) { // F8
					// 	}
					// }
				} else if (buffer[2] == 0x32) {
					if(buffer[3] == 0x30) {
						// if(buffer[4] == 0x7e) { // F9
						// }
					// } else if (buffer[3] == 0x31) {
					// 	if(buffer[4] == 0x7e) { // F10
					// 	}
					// } else if (buffer[3] == 0x34) {
					// 	if(buffer[4] == 0x7e) { // F12
					// 	}
					} else if (buffer[3] == 0x7e) { // Insert
						INSERT_MODE = !INSERT_MODE;
					}
				} else if (buffer[2] == 0x33) {
					if(buffer[3] == 0x7e) { // Delete
						if (cursor < scursor) {
							int remsize = remove_multichar(line, size, pos);
							printf("%s ", &line[pos]);
							scursor -= 1;
							move_cursor_left(scursor-cursor+1);
							size -= remsize;
						}
					}
				// } else if(buffer[2] == 0x41) { // Seta Cima
				// } else if(buffer[2] == 0x42) { // Seta Baixo
				} else if(buffer[2] == 0x43) { // Seta Direita
					if (cursor < scursor) { 
						pos = move_right(line, pos);
						move_cursor_rigth(1);
						cursor += 1;
					}
				} else if(buffer[2] == 0x44) { // Seta Esquerda
					if (cursor > 0) { 
						pos = move_left(line, pos);
						move_cursor_left(1);
						cursor -= 1;
					}
				}
			// } else if (buffer[1] == 0x4f) {
			// 	if (buffer[2] == 0x50) { // F1
			// 	} else if (buffer[2] == 0x51) { // F2
			// 	} else if (buffer[2] == 0x52) { // F3
			// 	} else if (buffer[2] == 0x53) { // F4
			// 	}
			}
		// } else if (buffer[0] == 0x1c) { // FS
		// } else if (buffer[0] == 0x1d) { // GS
		// } else if (buffer[0] == 0x1e) { // RS
		// } else if (buffer[0] == 0x1f) { // US
		} else if (buffer[0] >= 0x20 && buffer[0] <= 0x7e) { // CHARS
			if (INSERT_MODE) {
				int repsize = replace_multichar(line, buffer, size, bsize, pos);
				printf("%s", &line[pos]);
				move_cursor_left(scursor-cursor-1);
				pos += bsize;
				size += bsize;
				size -= repsize;
				cursor += 1;
			} else {
				put_multichar(line, buffer, size, bsize, pos);
				printf("%s", &line[pos]);
				move_cursor_left(scursor-cursor);
				pos += bsize;
				size += bsize;
				cursor += 1;
				scursor += 1;
			}
		} else if (buffer[0] == 0x7f) { // DEL
			if (cursor > 0) {
				pos = move_left(line, pos);
				int remsize = remove_multichar(line, size, pos);
				move_cursor_left(1);
				printf("%s ", &line[pos]);
				move_cursor_left(scursor-cursor+1);
				size -= remsize;
				scursor -= 1;
				cursor -= 1;
			}
		} else {
			if (INSERT_MODE) {
				int repsize = replace_multichar(line, buffer, size, bsize, pos);
				printf("%s", &line[pos]);
				move_cursor_left(scursor-cursor-1);
				pos += bsize;
				size += bsize;
				size -= repsize;
				cursor += 1;
			} else {
				put_multichar(line, buffer, size, bsize, pos);
				printf("%s", &line[pos]);
				move_cursor_left(scursor-cursor);
				pos += bsize;
				size += bsize;
				cursor += 1;
				scursor += 1;
			}
		}

		fflush(stdout);
	} while(1);
}
