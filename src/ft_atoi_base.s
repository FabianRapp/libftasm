
;int ft_atoi_base(char *str, char *base_str);

global ft_atoi_base
extern ft_strlen
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

	; if the first char in the base is \0 the base is invalid
	mov dil, [rbx]
	test rdi, rdi
	jz .ret_false

.loop:
	; rdi = c = *base_str
	mov dil, [rbx]
	; if !c return true
	cmp dil, 0
	je .ret_true

	; if ft_isspace(c) return false
	push rcx
	push rdi
	call ft_isspace
	pop rdi
	pop rcx
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
	jnz .ret_false
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
;	[x] int ret = 0;
;	[x] int base = strlen(base_str);
;	[ ] int i = 0;
;	[x] int sign = 1;
;	[x] if (!valid_base(base_str)) {
;	[x] 	return false;
;	[x] }
;	[x] while (ft_isspace(*str)) {
;	[x] 	str++;
;	[x] }
;	[x] if (str[0] == '-') {
;	[x] 	sign = -1;
;	[x] 	str++;
;	[x] } else if (*str == '+') {
;	[x] 	str++;
;	[x] }
;	[ ] for (int i = 0; str[i]; i++) {
;	[ ] 	int digit = 0;
;	[ ] 	for (; digit < base; digit++) {
;	[ ] 		if (str[i] == base_str[digit]) {
;	[ ] 			break ;
;	[ ] 		}
;	[ ] 	}
;	[x] 	if (digit == base) { // c is not in base_str -> return number
;	[x] 		return ret * sign;
;	[x] 	}
;	[x] 	ret *= base;
;	[x] 	ret += digit;
;	[x] }
;	[x] return ret * sign;
;}

ft_atoi_base:
	;test for NULL ptrs
	test rdi, rdi
	jz .ret_0_0
	test rsi, rsi
	jz .ret_0_0


	push rbx ; push old_rbx
	push rsi ; push base
	push rdi ; push str

	mov rdi, rsi
	call valid_base
	pop rdi ; pop str
	test rax, rax
	jz .ret_0_1

	mov rbx, rdi ; -> rbx is now the number-string

.skip_space:
	xor rdi, rdi
	mov dil, [rbx]
	inc rbx

	test rdi, rdi ; if !*str -> return 0
	jz .ret_0_1

	call ft_isspace
	test rax, rax
	jnz .skip_space

	dec rbx ; since the above loop increases once too much

	pop rdi; get the base_str for strlen
	push rdi
	call ft_strlen

	pop rsi ;pop base

	push r8
	mov r8, rax ; r8 will now hold the base (strlen(base_str))

	push r9 ; r9 will hold the result
	xor r9, r9

	push r10 ;r10 will hold the sign for the return result
	mov r10, 1

	xor rdi, rdi
	mov dil, [rbx] ; dil is now the current char c

	push r11 ; push old r11 so it can be used as digit
	xor r11, r11

	cmp dil, '+'
	je .inc_and_loop

	cmp dil, '-'
	jne .main_loop

	mov r10, -1 ;sign = -1

.inc_and_loop:
	inc rbx
	jmp .main_loop



	; ======================================
	; MAIN LOOP
	; rbx == current pos in number str
	; dil(rdi) == current char c ([rbx])
	; rsi == base_str
	; r8 == base magnitude
	; r9 == current number result
	; r11 == current digit
	; ======================================
.main_loop:
	xor rdi, rdi
	mov dil, [rbx] ; dil is now the current char c

	test rdi, rdi ; if !*str return
	jz .ret_result

	xor r11, r11 ; set digit to 0

	.inner_loop:
		cmp r8, r11 ; if base_magnitude <= digit
		jle .ret_result ; return

		cmp dil, byte [rsi + r11] ; if c == base[digit]
		je .break_inner ; break

		inc r11 ; digit++
		jmp .inner_loop


.break_inner:

	imul r9, r8 ; result *= base_magnitude
	add r9, r11 ; result += digit

	inc rbx ; str++
	jmp .main_loop



; ====================================
; success returns
.ret_result:
	imul r9, r10 ; result = result * sign
	mov rax, r9
	pop r11
	pop r10
	pop r9
	pop r8
.ret:
	pop rbx ;pop old_rbx
	ret

; ====================================
; error retruns -> return 0
.ret_0_1:
	pop rsi ;pop base
	pop rbx ;pop old_rbx
.ret_0_0:
	xor rax, rax
	ret


