#include "libftasm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char *strgenerator(int idx) {
	if (idx == 0) {
		return strdup("");
	} else if (idx == 1) {
		return strdup("Hello World!");
	} else if (idx == 2) {
		char *str = calloc(1, 1000);
		for (int i = 0; i < 1000; i++) {
			str[i] = 'B';
		}
		return str;
	}

	char *str = calloc(1, 1000);
	for (int i = 0; i < 1000; i++) {
		char c = rand() % 256;
		str[i] = c;
	}
	return str;
}

void test_strs(int *passes, int *fails) {
	for (int i = 0; i < 100; i++) {
		char *str = strgenerator(i);
		size_t std = strlen(str);
		size_t mine = ft_strlen(str);
		if (mine == std) {
			(*passes)++;
		} else {
			(*fails)++;
			printf("|%s|: strlen() = %lu; ft_strlen() = %lu\n", str, strlen(str), ft_strlen(str));
		}
		free(str);
	}
}

int main(void) {
	int passes = 0;
	int fails = 0;
	srand(time(NULL));
	test_strs(&passes, &fails);

	printf("%d passes and %d fails\n", passes, fails);
	return 0;
}
