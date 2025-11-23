;char *ft_strdup(const char *str);
section .text
global ft_strdup
extern ft_strlen
extern ft_strcpy
extern malloc

ft_strdup:
	push rdi;push str

	; len = strlen(str)
	call ft_strlen

	;new = malloc(len + 1)
	inc rax
	mov rdi, rax
	call malloc wrt ..plt
	test rax, rax
	jz .malloc_error

.malloc_success:
	;ft_strcpy(new, str)
	mov rdi, rax
	pop rsi

	push rdi
	call ft_strcpy

	pop rax
	ret


.malloc_error:
	pop rcx
	ret

