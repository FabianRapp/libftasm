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

void test_strlen(int *passes, int *fails, char *str) {
	size_t std = strlen(str);
	size_t mine = ft_strlen(str);
	if (mine == std) {
		(*passes)++;
	} else {
		(*fails)++;
		printf("|%s|: strlen() = %lu; ft_strlen() = %lu\n", str, strlen(str), ft_strlen(str));
	}
}

void test_cpy(int *passes, int *fails, char *str) {
	size_t len = strlen(str) + 1000;
	char *new_std = malloc(len);
	char *new_mine = malloc(len);
	memcpy(new_mine, new_std, len);

	char *std = strcpy(new_std, str);
	char *mine = ft_strcpy(new_mine, str);
	//todo: test return value
	if (!strcmp(new_std, new_mine)) {
		(*passes)++;
	} else {
		(*fails)++;
		printf("FAIL: strcpy: %s; ft_strcpy: %s\n", new_std, new_mine);
	}
	int diff_std = std - new_std;
	int diff_mine = mine - new_mine;
	if (diff_std != diff_mine) {
		(*fails)++;
		printf("FAIL(|%s|): term_idx: strcpy %d; ft_strcpy: %d\n", str, diff_std, diff_mine);
	}
	free(new_std);
	free(new_mine);
}

void test_strs(int *passes, int *fails) {
	for (int i = 0; i < 5; i++) {
		char *str1 = strgenerator(i);
		test_strlen(passes, fails, str1);
		test_cpy(passes, fails, str1);
		free(str1);
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
