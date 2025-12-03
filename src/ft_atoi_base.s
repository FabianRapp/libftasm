
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


;bool valid_base(chae *base_str) {
;	[x] char mem[256];
;	[x] memset(mem, 0, 256);
;
;	[x] for (int i = 0; base_str[i]; i++) {
;	 	if (
;	[ ]			mem[base_str[i]] 
;	[x]			|| base_str[i] == '+' 
;	[x]			|| base_str[i] == '-' 
;	[x]			|| ft_isspace(base_str[i])) {
;	[x] 		return false
;	 	}
;	[ ] 	mem[base_str[i]] = 1;
;	}
;	[x] return true;
;}
valid_base:
	push rbx
	; set rbx as base_str
	mov rbx, rdi

	;char mem(rcx)[256];
	sub rsp, 256
	mov rcx, rsp
	mov rax, 256
	;memset(mem, 0, 256)

.memset_loop:
	mov byte [rcx], 0
	dec rax
	inc rcx
	test rax, rax
	jnz .memset_loop

	sub rcx, 256
	xor rdi, rdi; for ft_isspace args zero the upper bytes

.loop:
	; c= *base_str
	mov dil, [rbx]
	; if !c return true
	cmp dil, 0
	je .ret_true

	; if ft_isspace(c) return false
	push rcx
	push rdi
	call ft_isspace
	pop rcx
	pop rdi
	test rax, rax
	jnz .ret_false

	cmp dil, '-'
	je .ret_false
	cmp dil, '+'
	je .ret_false

; if mem[c] ret false
	add rcx, rdi
	xor rax, rax
	mov al, [rcx]
	test rax, rax
	jz .ret_false
; mem[c] = 1
	mov byte [rcx], 1
	sub rcx, rdi


	;base_str++
	inc rbx
	jmp .loop

.ret_true:
	add rsp, 256
	pop rbx
	mov rax, 1
	ret
.ret_false:
	add rsp, 256
	pop rbx
	mov rax, 0
	ret

;int ref_ft_atoi_base(char *str, char *base_str) {
;	[ ] int ret = 0;
;	[ ] int base = strlen(base_str);
;	[ ] int i = 0;
;	[ ] int sign = 1;
;	[x] if (!valid_base(base_str)) {
;	[x] 	return false;
;	[x] }
;	[ ] while (ft_isspace(*str)) {
;	[ ] 	str++;
;	[ ] }
;	[ ] if (str[0] == '-') {
;	[ ] 	sign = -1;
;	[ ] 	str++;
;	[ ] } else if (*str == '+') {
;	[ ] 	str++;
;	[ ] }
;	[ ] for (int i = 0; str[i]; i++) {
;	[ ] 	int digit = 0;
;	[ ] 	for (; digit < base; digit++) {
;	[ ] 		if (str[i] == base_str[digit]) {
;	[ ] 			ret *= base;
;	[ ] 			ret += digit;
;	[ ] 			break ;
;	[ ] 		}
;	[ ] 	}
;	[ ] 	if (digit == base) {
;	[ ] 		return ret * sign;
;	[ ] 	}
;	[ ] }
;	[ ] return ret * sign;
;}
ft_atoi_base:
	push rdi
	push rsi
	mov rsi, rdi
	call valid_base
	pop rsi
	pop rdi

	test rax, rax
	jz .ret_0
	



.ret:
	xor rax, rax
	inc rax
	ret


.ret_0:
	xor rax, rax
	ret


