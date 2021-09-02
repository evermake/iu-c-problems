#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK 16

char shift_letter(char c, int n) {
	char result = c;
	if (65 <= c && c <= 90) {
		result = (((c - 65) + n) % 26) + 65;
	} else if (97 <= c && c <= 122) {
		result = (((c - 97) + n) % 26) + 97;
	}
	return result;
}

void memory_error() {
	fprintf(stderr, "Out of memory!");
	exit(1);
}

int main () {
	FILE *fi, *fo;

	size_t line_size = CHUNK, text_l = 0, line_l = 0, last_line_l = 0;
	char *line = malloc(line_size), *text = NULL, *last_line = NULL;

	if (line == NULL) memory_error();

	fi = fopen("input.txt", "r");
	fo = fopen("output.txt", "w");

	if (fi == NULL || fo == NULL) {
		printf("Failed to open the file!\n");
		exit(2);
	}

	char ch;
	while ((ch = getc(fi)) != EOF) {
		// Allocate more memory, if necessary
		if (line_l >= line_size - 1) {
			line_size += CHUNK;
			line = realloc(line, line_size);
			if (line == NULL) memory_error();
		}

		line[line_l++] = ch;

		if (ch == '\n') {
			if (last_line != NULL) {
				text_l += last_line_l;
				text = realloc(text, text_l);
				if (text == NULL) memory_error();
				strcat(text, last_line);
			}

			last_line_l = line_l;
			last_line = malloc(last_line_l);
			if (last_line == NULL) memory_error();
			for (int i = 0; i < line_l; i++) {
				last_line[i] = line[i];
			}

			line_l = 0;
			line_size = CHUNK;
			line = malloc(line_size);
			if (line == NULL) memory_error();
		}
	}

	int shift;
	sscanf(last_line, "%d", &shift);
	for (int i = 0; i < text_l; i++) {
		putc(shift_letter(text[i], shift), fo);
	}

	fclose(fi);
	fclose(fo);
}

