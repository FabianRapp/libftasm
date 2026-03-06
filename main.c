#include "libasm.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

int passes = 0;
int fails = 0;


/* todo:
 [x] - ft_atoi_base.s
 [x] - ft_list_push_front.s
 [x] - ft_list_remove_if.s
 [x] - ft_list_size.s
 [x] - ft_list_sort.s
 [x] - ft_read.s
 [x] - ft_strcmp.s
 [x] - ft_strcpy.s
 [x] - ft_strdup.s
 [x] - ft_strlen.s
 [x] - ft_write.s
 [x] - remove passes/fails pointer passing and simply make them global
 [x] - valgrind issues with some test cases
 [x] - compile warning:
	-> /usr/bin/ld: warning: ft_strlen.o: missing .note.GNU-stack section implies executable stack
	-> /usr/bin/ld: NOTE: This behaviour is deprecated and will be removed in a future version of the linker
 [x] - rename bonus files with '_bonus' suffix
*/

bool ft_isspace(char c) {
	return (c >= 9 && c <= 13) || (c == ' ');
}

bool ref_valid_base(char *base) {
	if (!base || !*base) {
		return false;
	}
	int len = ft_strlen(base);
	char mem[256];
	memset(mem, 0, 256);
	for (int i = 0; i < len; i++) {
		if (mem[base[i]] || base[i] == '+' || base[i] == '-' || ft_isspace(base[i])) {
			//invalid base
			return false;
		}
		mem[base[i]] = 1;
	}
	return true;
}

int ref_ft_atoi_base(char *str, char *base_str) {
	if (!str || !base_str) {
		return 0;
	}
	int ret = 0;
	int base = strlen(base_str);
	//if (base == 1) {
	//	return 0;
	//}
	int i = 0;
	int sign = 1;
	if (!ref_valid_base(base_str)) {
		return 0;
	}
	while (ft_isspace(*str)) {
		str++;
	}
	if (str[0] == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}
	for (int i = 0; str[i]; i++) {
		int digit = 0;
		for (; digit < base; digit++) {
			if (str[i] == base_str[digit]) {
				ret *= base;
				ret += digit;
				break ;
			}
		}
		if (digit == base) {
			return ret * sign;
		}
	}
	return ret * sign;
}

#ifdef BONUS
void test_one_atoi_base(const char *s, const char *b) {
	int ref = ref_ft_atoi_base((char *)s, (char *)b);
	int got = ft_atoi_base((char *)s, (char *)b);

	if (ref != got) {
		fails++;
		printf("FAIL: ft_atoi_base(\"%s\", \"%s\") -> got %d, expected %d\n",
			   s, b, got, ref);
	} else {
		//printf("PASS: ft_atoi_base(\"%s\", \"%s\") -> got %d, expected %d\n",
		//	   s, b, got, ref);
		passes++;
	}
}

void test_atoi_base_basic(void) {
	test_one_atoi_base("", "0123456789");
	test_one_atoi_base("2", "0123456789");
	test_one_atoi_base("+2", "0123456789");
	test_one_atoi_base("-2", "0123456789");
	test_one_atoi_base("0", "0123456789");
	test_one_atoi_base("2", "12");
	test_one_atoi_base(NULL, "12");
	test_one_atoi_base("42", "0123456789");
	test_one_atoi_base("-42", "0123456789");
	test_one_atoi_base("+42", "0123456789");
	test_one_atoi_base("123", "0123456789");
	test_one_atoi_base("\t\n\r\v\f  -123", "0123456789");
	test_one_atoi_base("   ---+--+1234ab567", "0123456789");

	test_one_atoi_base("10102", "01");
	test_one_atoi_base("FFG", "0123456789ABCDEF");
	test_one_atoi_base("   +--+xyz", "0123456789");

	test_one_atoi_base("p", "poneyvif");
	test_one_atoi_base("yv", "poneyvif");
	test_one_atoi_base("vvvv", "poneyvif");
	test_one_atoi_base("-poney", "poneyvif");

	test_one_atoi_base("FF", "0123456789ABCDEF");

	test_one_atoi_base("abc", "0123456789");

	test_one_atoi_base("+-+-", "0123456789");
	test_one_atoi_base("   -+--+0", "0123456789");
}

static void test_atoi_base_invalid_bases(void) {
	test_one_atoi_base("123", NULL);
	test_one_atoi_base(NULL, NULL);
	test_one_atoi_base("123", "");
	test_one_atoi_base("123", "0");

	test_one_atoi_base("123", "00");
	test_one_atoi_base("123", "00123456789");
	test_one_atoi_base("ABC", "01234A56789A");
	test_one_atoi_base("poney", "pponeyvif");

	test_one_atoi_base("123", "012+3456789");
	test_one_atoi_base("123", "012-3456789");
	test_one_atoi_base("123", "0 123456789");
	test_one_atoi_base("123", "0\t123456789");
}
#endif //BONUS

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

void test_cmp(char *str1, char *str2) {
	int std = strcmp(str1, str2);
	int mine = ft_strcmp(str1, str2);

	int ok = (std == 0 && mine == 0) || (std < 0 && mine < 0) || (std > 0 && mine > 0);

	if (!ok) {
		fails++;
		printf("FAIL: strcmp(%s, %s) -> std: %d; mine: %d\n",
			str1, str2, std, mine);
	} else {
		passes++;
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
		test_cmp(str1, str1);
		test_strdup(str1);
		for (int j = 0; j < iter_count; j++) {
			if (j == i) {
				continue ;
			}
			char *str2 = strgenerator(j);
			test_cmp(str1, str2);
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

#ifdef BONUS

void test_atoi_base(void) {
	test_atoi_base_basic();
	test_atoi_base_invalid_bases();
}

void ref_list_push_front(t_list **head, void *data) {
	if (!head) {
		return ;
	}
	t_list *node = malloc(sizeof(*node));
	if (!node) {
		return;
	}
	node->data = data;
	node->next = *head;
	*head = node;
}

void ref_list_remove_if(t_list **head, void *data_ref, int (*cmp)(void *data1, void *data), void (*free_fct)(void *data)) {
	if (!head) {
		return ;
	}
	while (*head && !cmp(data_ref, (*head)->data)) {
		t_list *old_head = *head;
		*head = old_head->next;
		free_fct(old_head->data);
		free(old_head);
	}
	if (!*head) {
		return ;
	}
	t_list *last = *head;
	t_list *cur = (*head)->next;
	while (cur) {
		if (!cmp(data_ref, cur->data)) {
			last->next = cur->next;
			free_fct(cur->data);
			free(cur);
		} else {
			last = cur;
		}
		cur = last->next;
	}
}

int ref_list_size(t_list *head) {
	int size = 0;
	while (head) {
		size++;
		head = head->next;
	}
	return size;
}

void ref_list_sort(t_list **head, int (*cmp)(void *data1, void *data2)) {
	if (!head || !*head) {
		return ;
	}
	int len = ft_list_size(*head);
	while (--len) {
		int inner_iter_count = len;
		t_list *cur = *head;
		t_list *next = cur->next;
		while (inner_iter_count--) {
			if (cmp(cur->data, next->data) > 0) {
				cur->data = (void*)((uintptr_t)cur->data ^ (uintptr_t)next->data);
				next->data = (void*)((uintptr_t)cur->data ^ (uintptr_t)next->data);
				cur->data = (void*)((uintptr_t)cur->data ^ (uintptr_t)next->data);
			}
			cur = next;
			next = cur->next;
		}
	}
}

t_list *inc_list(int len) {
	if (len <= 0) {
		return NULL;
	}
	t_list *head = malloc(sizeof *head);
	head->data = malloc(sizeof(int));
	*(int*)(head->data) = 0;
	head->next = NULL;
	t_list *cur = head;

	for (int i = 1; i < len; i++) {
		cur->next = calloc(sizeof(t_list), 1);
		cur = cur->next;
		cur->data = malloc(sizeof(int));
		*(int*)cur->data = i;
		cur->next = NULL;
	}
	return head;
}

void free_list(t_list *head) {
	if (!head) {
		return ;
	}
	t_list *cur = head->next;
	t_list *last = head;
	while (cur) {
		free(last->data);
		free(last);
		last = cur;
		cur = cur->next;
	}
	free(last->data);
	free(last);
}

void test_list_size() {
	for (int i = 0; i < 5; i++) {
		t_list *a = inc_list(i);
		int ref = ref_list_size(a);
		int actual = ft_list_size(a);
		if (ref != actual) {
			fails++;
			printf("FAIL: ft_list_size(list len %d): actual: %d\n", i, actual);
		} else {
			passes++;
		}
		free_list(a);
	}
}

bool cmp_list(t_list *a, t_list *b) {
	while (a && b) {
		if (!a->data && !b->data) {
			a = a->next;
			b = b->next;
			continue;
		} else if (!a->data || !b->data) {
			return false;
		} else if (*(int*)(a->data) != *(int*)(b->data)) {
			return false;
		}
		a = a->next;
		b = b->next;
	}
	if (!a && !b) {
		return true;
	}
	if (!a || !b) {
		return false;
	}
	assert(0);
}

void test_push_front() {
	errno = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = -1; j < 5; j++) {
			t_list *a = inc_list(i);
			t_list *b = inc_list(i);
			int *data_a = malloc(sizeof (int));
			int *data_b = malloc(sizeof (int));
			if (errno) {
				perror(strerror(errno));
				exit(errno);
			}
			*data_a = j;
			*data_b = j;
			ref_list_push_front(&a, data_a);
			ft_list_push_front(&b, data_b);
			if (cmp_list(a, b)) {
				passes++;
			} else {
				printf("FAIL: ft_list_push_front(i: %d; j: %d)\n", i, j);
				fails++;
			}
			free_list(a);
			free_list(b);
		}
	}
}

int cmp_int(void *a, void *b) {
	if (!a && !b) {
		return 0;
	}
	if (!a) {
		return -1;
	}
	if (!b) {
		return 1;
	}
	return *(int *)a - *(int *)b;
}

t_list *dup_list(const t_list *src) {
	t_list *head = NULL;
	t_list *tail = NULL;

	while (src) {
		t_list *n = (t_list *)calloc(1, sizeof(*n));
		if (src->data) {
			int *v = (int *)malloc(sizeof(int));
			*v = *(int *)src->data;
			n->data = v;
		} else {
			n->data = NULL;
		}
		n->next = NULL;
		if (!head) {
			head = n;
		} else {
			tail->next = n;
		}
		tail = n;
		src = src->next;
	}
	return head;
}

int count_value(t_list *list, int value) {
	int c = 0;
	while (list) {
		if (list->data && *(int *)list->data == value) {
			c++;
		}
		list = list->next;
	}
	return c;
}

void print_list(const char *tag, t_list *head) {
	printf("%s (len=%d): ", tag, ref_list_size(head));
	while (head) {
		if (!head->data) {
			printf("[NULL] ");
		} else {
			printf("%d ", *(int*)head->data);
		}
		head = head->next;
	}
	printf("\n");
}

bool list_eq(t_list *a, t_list *b) {
	while (a && b) {
		if ((!a->data) != (!b->data)) {
			return false;
		}
		if (a->data && b->data && *(int*)a->data != *(int*)b->data) {
			return false;
		}
		a = a->next;
		b = b->next;
	}
	return a == NULL && b == NULL;
}

int list_has_cycle(t_list *head) {
	t_list *slow = head;
	t_list *fast = head;
	while (fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast) {
			return 1;
		}
	}
	return 0;
}

void print_diff(t_list *exp, t_list *got) {
	int idx = 0;
	while (exp && got) {
		int ev = exp->data ? *(int*)exp->data : 0;
		int gv = got->data ? *(int*)got->data : 0;

		if ((!exp->data) != (!got->data)) {
			printf("  first diff @idx=%d: expected %s, got %s\n",
				idx, exp->data ? "value" : "NULL", got->data ? "value" : "NULL");
			return;
		}
		if (exp->data && got->data && ev != gv) {
			printf("  first diff @idx=%d: expected %d, got %d\n", idx, ev, gv);
			return;
		}
		exp = exp->next;
		got = got->next;
		idx++;
	}
	if (!exp && got) {
		printf("  got has extra nodes starting @idx=%d (next=%p)\n", idx, (void*)got);
	} else if (exp && !got) {
		printf("  got ended early @idx=%d (expected continues, next=%p)\n", idx, (void*)exp);
	} else {
		assert(0);
	}
}

void debug_remove_if_case(const char *label, t_list *base, int refv, t_list *exp, t_list *got) {
	printf("\n=== %s (ref=%d) ===\n", label, refv);

	print_list("base", base);
	print_list("exp ", exp);
	print_list("got ", got);

	printf("checks: eq=%s, got_cycle=%s, exp_cycle=%s\n",
		list_eq(exp, got) ? "YES" : "NO",
		list_has_cycle(got) ? "YES" : "NO",
		list_has_cycle(exp) ? "YES" : "NO"
	);

	print_diff(exp, got);
}

void test_remove_if(void) {
	{
		int refv = 0;
		ref_list_remove_if(NULL, &refv, cmp_int, free);
		ft_list_remove_if(NULL, &refv, cmp_int, free);
		passes++;
	}

	{
		t_list *a = NULL;
		t_list *b = NULL;
		int refv = 0;
		ref_list_remove_if(&a, &refv, cmp_int, free);
		ft_list_remove_if(&b, &refv, cmp_int, free);
		if (a == NULL && b == NULL) {
			passes++;
		} else {
			fails++;
			printf("FAIL: ft_list_remove_if on empty list\n");
		}
	}

	for (int len = 0; len <= 8; len++) {
		t_list *base = inc_list(len);
		t_list *a = dup_list(base);
		t_list *b = dup_list(base);

		int refv = 9999;
		ref_list_remove_if(&a, &refv, cmp_int, free);
		ft_list_remove_if(&b, &refv, cmp_int, free);

		if (cmp_list(a, b)) {
			passes++;
		} else {
			fails++;
			debug_remove_if_case("remove none", base, refv, a, b);
		}

		free_list(base);
		free_list(a);
		free_list(b);
	}

	for (int len = 1; len <= 8; len++) {
		t_list *base = inc_list(len);
		t_list *a = dup_list(base);
		t_list *b = dup_list(base);

		int refv = 0;
		ref_list_remove_if(&a, &refv, cmp_int, free);
		ft_list_remove_if(&b, &refv, cmp_int, free);

		if (cmp_list(a, b)) {
			passes++;
		} else {
			fails++;
			debug_remove_if_case("remove head", base, refv, a, b);
		}

		free_list(base);
		free_list(a);
		free_list(b);
	}

	for (int len = 1; len <= 8; len++) {
		t_list *base = inc_list(len);
		t_list *a = dup_list(base);
		t_list *b = dup_list(base);

		int refv = len - 1;
		ref_list_remove_if(&a, &refv, cmp_int, free);
		ft_list_remove_if(&b, &refv, cmp_int, free);

		if (cmp_list(a, b)) {
			passes++;
		} else {
			fails++;
			debug_remove_if_case("remove tail", base, refv, a, b);
		}

		free_list(base);
		free_list(a);
		free_list(b);
	}

	for (int len = 3; len <= 9; len++) {
		t_list *base = inc_list(len);
		t_list *a = dup_list(base);
		t_list *b = dup_list(base);

		int refv = len / 2;
		ref_list_remove_if(&a, &refv, cmp_int, free);
		ft_list_remove_if(&b, &refv, cmp_int, free);

		if (cmp_list(a, b)) {
			passes++;
		} else {
			fails++;
			debug_remove_if_case("remove middle", base, refv, a, b);
		}

		free_list(base);
		free_list(a);
		free_list(b);
	}

	for (int len = 0; len <= 8; len++) {
		t_list *base = inc_list(len);
		t_list *a = dup_list(base);
		t_list *b = dup_list(base);

		for (int v = 0; v < len; v++) {
			int refv = v;
			ref_list_remove_if(&a, &refv, cmp_int, free);
			ft_list_remove_if(&b, &refv, cmp_int, free);
		}

		if (a == NULL && b == NULL) {
			passes++;
		} else {
			fails++;
			printf("FAIL: ft_list_remove_if(remove all) len=%d (a=%p b=%p)\n", len, (void *)a, (void *)b);
		}

		free_list(base);
		free_list(a);
		free_list(b);
	}

	{
		int vals[] = {1, 2, 2, 2, 3, 2, 4};
		t_list *base = NULL;
		for (int i = (int)(sizeof(vals) / sizeof(vals[0])) - 1; i >= 0; i--) {
			int *p = (int *)malloc(sizeof(int));
			*p = vals[i];
			ref_list_push_front(&base, p);
		}

		t_list *a = dup_list(base);
		t_list *b = dup_list(base);

		int refv = 2;
		ref_list_remove_if(&a, &refv, cmp_int, free);
		ft_list_remove_if(&b, &refv, cmp_int, free);

		if (cmp_list(a, b) && count_value(b, 2) == 0) {
			passes++;
		} else {
			fails++;
			debug_remove_if_case("remove duplicates", base, refv, a, b);
		}

		free_list(base);
		free_list(a);
		free_list(b);
	}
	{
		int vals[] = {1, 1, 1, 2, 1, 1, 1};
		t_list *base = NULL;
		for (int i = (int)(sizeof(vals) / sizeof(vals[0])) - 1; i >= 0; i--) {
			int *p = (int *)malloc(sizeof(int));
			*p = vals[i];
			ref_list_push_front(&base, p);
		}

		t_list *a = dup_list(base);
		t_list *b = dup_list(base);

		int refv = 2;
		ref_list_remove_if(&a, &refv, cmp_int, free);
		ft_list_remove_if(&b, &refv, cmp_int, free);

		if (cmp_list(a, b) && count_value(b, 2) == 0) {
			passes++;
		} else {
			fails++;
			debug_remove_if_case("remove all but middle", base, refv, a, b);
		}

		free_list(base);
		free_list(a);
		free_list(b);
	}
}

void debug_sort_case(const char *label, t_list *base, t_list *exp, t_list *got) {
	printf("\n=== %s ===\n", label);
	print_list("base", base);
	print_list("exp ", exp);
	print_list("got ", got);

	printf("checks: eq=%s, got_cycle=%s, exp_cycle=%s, len_exp=%d len_got=%d\n",
		list_eq(exp, got) ? "YES" : "NO",
		list_has_cycle(got) ? "YES" : "NO",
		list_has_cycle(exp) ? "YES" : "NO",
		ref_list_size(exp), ref_list_size(got)
	);

	print_diff(exp, got);

	if (list_has_cycle(got)) {
		printf("hint: got has a cycle; common causes are wrong next rewiring or missing tail->next = NULL.\n");
	}
	if (ref_list_size(exp) != ref_list_size(got)) {
		printf("hint: length changed; common causes are losing nodes, overwriting begin_list, or skipping links.\n");
	}
}

t_list *make_list_from_ints_with_nulls(const int *vals, const int *is_null, int n) {
	t_list *h = NULL;
	for (int i = n - 1; i >= 0; i--) {
		if (is_null && is_null[i]) {
			ref_list_push_front(&h, NULL);
		} else {
			int *p = (int *)malloc(sizeof(int));
			*p = vals[i];
			ref_list_push_front(&h, p);
		}
	}
	return h;
}

t_list *make_list_from_ints(const int *vals, int n) {
	return make_list_from_ints_with_nulls(vals, NULL, n);
}

void run_sort_case(const char *label, t_list *base) {
	t_list *exp = dup_list(base);
	t_list *got = dup_list(base);

	ref_list_sort(&exp, cmp_int);
	ft_list_sort(&got, cmp_int);

	bool ok = true;

	if (list_has_cycle(got)) {
		ok = false;
	}
	if (ref_list_size(exp) != ref_list_size(got)) {
		ok = false;
	}
	if (!cmp_list(exp, got)) {
		ok = false;
	}

	if (ok) {
		passes++;
	} else {
		fails++;
		debug_sort_case(label, base, exp, got);
	}

	free_list(base);
	free_list(exp);
	free_list(got);
}

void test_list_sort(void) {
	{
		ref_list_sort(NULL, cmp_int);
		ft_list_sort(NULL, cmp_int);
		passes++;
	}

	{
		t_list *a = NULL;
		t_list *b = NULL;
		ref_list_sort(&a, cmp_int);
		ft_list_sort(&b, cmp_int);
		if (a == NULL && b == NULL) {
			passes++;
		} else {
			fails++; printf("FAIL: ft_list_sort on empty list\n");
		}
	}

	{
		int v[] = {42};
		run_sort_case("single", make_list_from_ints(v, 1));
	}

	{
		int v[] = {0,1,2,3,4,5,6};
		run_sort_case("already sorted", make_list_from_ints(v, (int)(sizeof(v)/sizeof(v[0]))));
	}

	{
		int v[] = {6,5,4,3,2,1,0};
		run_sort_case("reverse", make_list_from_ints(v, (int)(sizeof(v)/sizeof(v[0]))));
	}

	{
		int v[] = {3,1,2,2,5,3,3,0,2};
		run_sort_case("duplicates", make_list_from_ints(v, (int)(sizeof(v)/sizeof(v[0]))));
	}

	{
		int v[] = {0,-1,5,-10,3,-1,2};
		run_sort_case("negatives", make_list_from_ints(v, (int)(sizeof(v)/sizeof(v[0]))));
	}

	for (int it = 0; it < 200; it++) {
		int n = (rand() % 15); // 0..14
		int *vals = (int *)malloc((size_t)n * sizeof(int));
		if (!vals) {
			break;
		}

		for (int i = 0; i < n; i++) {
			vals[i] = (rand() % 21) - 10; // -10..10
		}

		t_list *base = make_list_from_ints(vals, n);

		char label[64];
		snprintf(label, sizeof(label), "random small #%d (n=%d)", it, n);
		run_sort_case(label, base);

		free(vals);
	}

	{
		int v[] = {5, 1, 3, 2, 4, 0};
		int is_null[] = {0, 1, 0, 0, 1, 0};
		t_list *base = make_list_from_ints_with_nulls(v, is_null, 6);
		run_sort_case("with NULL data", base);
	}
}

void test_lists() {
	test_list_size();
	test_push_front();
	test_remove_if();
	test_list_sort();
}
#endif // BONUS

int main(void) {
	srand(time(NULL));
	test_strs();
	test_data();

#ifdef BONUS
	test_atoi_base();
	test_lists();
#endif // BONUS

	printf("%d passes and %d fails\n", passes, fails);
	return 0;
}
