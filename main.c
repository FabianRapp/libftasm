#include "libftasm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/* todo:
 [ ] - ft_atoi_base.s
 [ ] - ft_list_push.s
 [ ] - ft_list_remove_if.s
 [ ] - ft_list_size.s
 [ ] - ft_list_sort.s
 [ ] - ft_read.s
 [x] - ft_strcmp.s
 [x] - ft_strcpy.s
 [ ] - ft_strdup.s
 [x] - ft_strlen.s
 [ ] - ft_write.s
 [ ] - errno test wrapper
 [x] - remove passes/fails pointer passing and simply make them global
 [ ] - compile warning:
	-> /usr/bin/ld: warning: ft_strlen.o: missing .note.GNU-stack section implies executable stack
	-> /usr/bin/ld: NOTE: This behaviour is deprecated and will be removed in a future version of the linker
 [ ] - rename bonus files with '_bonus' suffix
*/
int passes = 0;
int fails = 0;


char *strgenerator(int idx) {
	if (idx == 0) {
		return strdup("");
	} else if (idx == 1) {
		return strdup("Hello World!");
	} else if (idx == 2) {
		char *str = calloc(1, 1001);
		for (int i = 0; i < 1000; i++) {
			str[i] = 'B';
		}
		return str;
	}

	char *str = calloc(1, 1001);
	for (int i = 0; i < 1000; i++) {
		char c = rand() % 256;
		str[i] = c;
	}
	return str;
}

void test_strlen(char *str) {
	size_t std = strlen(str);
	size_t mine = ft_strlen(str);
	if (mine == std) {
		passes++;
	} else {
		fails++;
		printf("|%s|: strlen() = %lu; ft_strlen() = %lu\n", str, strlen(str), ft_strlen(str));
	}
}

void test_cpy(char *str) {
	size_t len = strlen(str) + 1000;
	char *new_std = malloc(len);
	char *new_mine = malloc(len);
	memcpy(new_mine, new_std, len);

	char *std = strcpy(new_std, str);
	char *mine = ft_strcpy(new_mine, str);
	//todo: test return value
	if (!strcmp(new_std, new_mine)) {
		(passes)++;
	} else {
		(fails)++;
		printf("FAIL: strcpy: %s; ft_strcpy: %s\n", new_std, new_mine);
	}
	int diff_std = std - new_std;
	int diff_mine = mine - new_mine;
	if (diff_std != diff_mine) {
		(fails)++;
		printf("FAIL(|%s|): term_idx: strcpy %d; ft_strcpy: %d\n", str, diff_std, diff_mine);
	}
	free(new_std);
	free(new_mine);
}

void test_cmp_eq(char *str) {
	int std = strcmp(str, str);
	int mine = ft_strcmp(str, str);
	if (std != mine) {
		(fails)++;
		printf("FAIL: strcmp(%s, %s) -> mine: %d\n", str, str, mine);
	} else {
		(passes)++;
	}
}

void test_cmp_diff(char *str1, char *str2) {
	int std = strcmp(str1, str2);
	int mine = ft_strcmp(str1, str2);
	if (std != mine) {
		(fails)++;
		printf("FAIL: strcmp(%s, %s) -> std: %d; mine: %d\n", str1, str2, std, mine);
	} else {
		(passes)++;
	}
}

int make_file1(char *data, int data_size) {
	int fd = open("FILE1", O_WRONLY | O_CREAT | O_TRUNC);
	if (fd < 0) {
		perror(strerror(errno));
		exit(1);
	}
	write(fd, data, data_size);
	return fd;
}

void test_read(char *data, int data_size) {
	int fd = make_file1(data, data_size);
	//todo seek fd pos etc..
	char *std = calloc(1, data_size);
	char *mine = calloc(1, data_size);
	read(fd, std, data_size);
	read(fd, mine, data_size);
	free(std);
	free(mine);
	close(fd);
}

void test_strs() {
	int iter_count = 10;
	//for (int i = 0; i < iter_count; i++) {
	for (int i = 0; !fails; i++) {
		char *str1 = strgenerator(i);
		test_strlen(str1);
		test_cpy(str1);
		test_cmp_eq(str1);
		for (int j = 0; j < iter_count; j++) {
			if (j == i) {
				continue ;
			}
			char *str2 = strgenerator(j);
			test_cmp_diff(str1, str2);
			free(str2);
		}
		free(str1);
	}
}

int main(void) {
	srand(time(NULL));
	test_strs();

	printf("%d passes and %d fails\n", passes, fails);
	return 0;
}
