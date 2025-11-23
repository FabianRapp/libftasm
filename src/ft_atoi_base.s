
;int ft_atoi_base(char *str, char *base_str);

global ft_atoi_base
section .text


;bool ft_isspace(char c) {
;	return (c >= 9 && c <= 13) || (c == ' ');
;}
ft_isspace:
	cmp rdi, ' '
	je .ret_true
	cmp rdi, 9
	jb .ret_false
	cmp rdi, 13
	ja .ret_false
.ret_true:
	mov rax, 1
	ret
.ret_false:
	mov rax, 0
	ret


;int ref_ft_atoi_base(char *str, char *base_str) {
;	int ret = 0;
;	int base = strlen(base_str);
;	int i = 0;
;	int sign = 1;
;	char mem[256];
;	memset(mem, 0, 256);
;	for (int i = 0; i < base; i++) {
;		if (mem[base_str[i]] || base_str[i] == '+' || base_str[i] == '-' || ft_isspace(base_str[i])) {
;			//invalid base
;			return (0);
;		}
;		mem[base_str[i]] = 1;
;	}
;	while (ft_isspace(*str)) {
;		str++;
;	}
;	if (str[0] == '-') {
;		sign = -1;
;		str++;
;	} else if (*str == '+') {
;		str++;
;	}
;	for (int i = 0; str[i]; i++) {
;		int digit = 0;
;		for (; digit < base; digit++) {
;			if (str[i] == base_str[digit]) {
;				ret *= base;
;				ret += digit;
;				break ;
;			}
;		}
;		if (digit == base) {
;			return ret * sign;
;		}
;	}
;	return ret * sign;
;}
ft_atoi_base:

	ret



