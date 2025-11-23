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
 [x] - ft_read.s
 [x] - ft_strcmp.s
 [x] - ft_strcpy.s
 [x] - ft_strdup.s
 [x] - ft_strlen.s
 [x] - ft_write.s
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

char *datagenerator(int idx, int *size) {
	if (idx == 0) {
		*size = 0;
		return strdup("");
	} else if (idx == 1) {
		*size = strlen("Hello World!");
		return strdup("Hello World!");
	} else if (idx == 2) {
		char *str = calloc(1, 1001);
		for (int i = 0; i < 1000; i++) {
			str[i] = 'B';
		}
		*size = 1001;
		return str;
	}

	char *str = calloc(1, 1001);
	for (int i = 0; i < 1000; i++) {
		char c = rand() % 256;
		str[i] = c;
	}
	*size = 1001;
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
	int fd = open("FILE1", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0) {
		perror("Open");
		exit(1);
	}
	if (write(fd, data, data_size) < 0) {
		perror("write");
	}
	return fd;
}

void test_strdup(char *str) {
	char *std = strdup(str);
	char *mine1 = ft_strdup(str);
	char *mine2 = ft_strdup(str);

	if (!std || !mine1 || !mine2) {
		if (std == mine1 && mine1 == mine2) {
			passes++;
		} else {
			fails++;
			printf("FAIL: strdup(%s): NULL mismatch: std=%p mine1=%p mine2=%p\n",
			       str, (void *)std, (void *)mine1, (void *)mine2);
		}
		free(std);
		free(mine1);
		free(mine2);
		return ;
	}

	if (strcmp(std, mine1) || strcmp(std, mine2)) {
		fails++;
		printf("FAIL: strdup content mismatch for |%s|\n", str);
		printf("  std  : |%s|\n", std);
		printf("  mine1: |%s|\n", mine1);
		printf("  mine2: |%s|\n", mine2);
	} else {
		passes++;
	}

	if (mine1 == str) {
		fails++;
		printf("FAIL: ft_strdup(%s) returned same pointer as input\n", str);
	}
	if (mine2 == str) {
		fails++;
		printf("FAIL: second ft_strdup(%s) returned same pointer as input\n", str);
	}
	if (mine1 == mine2) {
		fails++;
		printf("FAIL: ft_strdup(%s) returned same pointer for two calls\n", str);
	}

	if (mine1[0] != '\0') {
		char old1 = mine1[0];
		char old2 = mine2[0];
		char olds = std[0];

		mine1[0] = (char)(mine1[0] ^ 0xFF);

		if (mine2[0] != old2) {
			fails++;
			printf("FAIL: ft_strdup(%s): copies not independent (mine2 changed)\n", str);
		}
		if (std[0] != olds) {
			fails++;
			printf("FAIL: ft_strdup(%s): std copy changed when mine1 modified\n", str);
		}

		mine1[0] = old1;
	}

	free(std);
	free(mine1);
	free(mine2);
}

void test_strs() {
	int iter_count = 100;
	for (int i = 0; i < iter_count; i++) {
	//for (int i = 0; !fails; i++) {
		char *str1 = strgenerator(i);
		test_strlen(str1);
		test_cpy(str1);
		test_cmp_eq(str1);
		test_strdup(str1);
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

void test_valid_read(char *data, int data_size) {
	int fd = make_file1(data, data_size);
	char *std = calloc(1, data_size * 2);
	char *mine = calloc(1, data_size * 2);
	lseek(fd, 0, SEEK_SET);
	int ret_std = read(fd, std, data_size);
	if (ret_std < 0) {
		perror("read");
	}
	lseek(fd, 0, SEEK_SET);
	int ret_mine = ft_read(fd, mine, data_size);
	if (ret_std != ret_mine) {
		fails++;
		printf("read return value error: std = %d; mine = %d\n", ret_std, ret_mine);
	}
	if (memcmp(std, mine, data_size * 2)) {
		printf("Read inccorect data\n");
		fails++;
	} else if (ret_std == ret_mine) {
		passes++;
	}
	free(std);
	free(mine);
	close(fd);
}

void test_invalid_read(char *data, int data_size) {
	int fd = make_file1(data, data_size);
	char *std = calloc(1, data_size * 2);
	char *mine = calloc(1, data_size * 2);
	close(fd);
	lseek(fd, 0, SEEK_SET);
	errno = 0;
	int ret_std = read(fd, std, data_size);
	int std_errno = errno;
	lseek(fd, 0, SEEK_SET);
	errno = 0;
	int ret_mine = ft_read(fd, mine, data_size);
	int mine_errno = errno;
	if (ret_std != ret_mine) {
		fails++;
		printf("read return value error: std = %d; mine = %d\n", ret_std, ret_mine);
	}
	if (std_errno != mine_errno) {
		fails++;
		printf("read errno value error: std = %d; mine = %d\n", std_errno, mine_errno);
	}
	if (memcmp(std, mine, data_size * 2)) {
		printf("Read inccorect data\n");
		fails++;
	} else if (ret_std == ret_mine) {
		passes++;
	}
	free(std);
	free(mine);
	close(fd);
}

int make_empty_file(const char *name) {
	int fd = open(name, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0) {
		perror("open");
		exit(1);
	}
	return fd;
}

void test_valid_write(char *data, int data_size) {
	int fd_std = make_empty_file("FILE_STD");
	int fd_mine = make_empty_file("FILE_MINE");

	errno = 0;
	int ret_std = write(fd_std, data, data_size);
	int std_errno = errno;

	errno = 0;
	int ret_mine = ft_write(fd_mine, data, data_size);
	int mine_errno = errno;

	if (ret_std != ret_mine) {
		fails++;
		printf("write return value error: std = %d; mine = %d\n", ret_std, ret_mine);
	}
	if (std_errno != mine_errno) {
		fails++;
		printf("write errno value error: std = %d; mine = %d\n", std_errno, mine_errno);
	}

	lseek(fd_std, 0, SEEK_SET);
	lseek(fd_mine, 0, SEEK_SET);

	char *buf_std = calloc(1, data_size * 2);
	char *buf_mine = calloc(1, data_size * 2);

	int rd_std = read(fd_std, buf_std, data_size * 2);
	int rd_mine = read(fd_mine, buf_mine, data_size * 2);

	if (rd_std != rd_mine) {
		fails++;
		printf("write/read size mismatch after write: std = %d; mine = %d\n", rd_std, rd_mine);
	} else if (memcmp(buf_std, buf_mine, data_size * 2)) {
		fails++;
		printf("Write inccorect data\n");
	} else if (ret_std == ret_mine && std_errno == mine_errno) {
		passes++;
	}

	free(buf_std);
	free(buf_mine);
	close(fd_std);
	close(fd_mine);
}

void test_invalid_write(char *data, int data_size) {
	int fd_std = make_empty_file("FILE_STD_INV");
	int fd_mine = make_empty_file("FILE_MINE_INV");

	close(fd_std);
	close(fd_mine);

	lseek(fd_std, 0, SEEK_SET);
	errno = 0;
	int ret_std = write(fd_std, data, data_size);
	int std_errno = errno;

	lseek(fd_mine, 0, SEEK_SET);
	errno = 0;
	int ret_mine = ft_write(fd_mine, data, data_size);
	int mine_errno = errno;

	if (ret_std != ret_mine) {
		fails++;
		printf("write return value error (invalid fd): std = %d; mine = %d\n", ret_std, ret_mine);
	}
	if (std_errno != mine_errno) {
		fails++;
		printf("write errno value error (invalid fd): std = %d; mine = %d\n", std_errno, mine_errno);
	}
	if (ret_std == ret_mine && std_errno == mine_errno) {
		passes++;
	}
}

void test_data() {
	int iter_count = 10;
	for (int i = 0; i < iter_count; i++) {
	//for (int i = 0; !fails; i++) {
		int data_size1;
		char *data1 = datagenerator(i, &data_size1);
		test_valid_read(data1, data_size1);
		test_invalid_read(data1, data_size1);
		test_valid_write(data1, data_size1);
		test_invalid_write(data1, data_size1);
		free(data1);
	}
}

int main(void) {
	srand(time(NULL));
	test_strs();
	test_data();

	printf("%d passes and %d fails\n", passes, fails);
	return 0;
}
