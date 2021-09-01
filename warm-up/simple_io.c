#include <stdio.h>

int main() {
	FILE *fi, *fo;
	char ch;

	fi = fopen("input.txt", "r");
	fo = fopen("output.txt", "w");

	if (fi == NULL || fo == NULL) {
		printf("Failed to open the file!\n");
	} else {
		while ((ch = fgetc(fi)) != EOF) {
			fputc(ch, fo);
		}
	}

	fclose(fi);
	fclose(fo);
}

