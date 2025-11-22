;char *ft_stpcpy(char *restrict dst, const char *restrict src);

section .text
	global ft_strcpy

ft_strcpy:
	mov rax, rdi
.loop:
	mov byte cl, [rsi]
	mov byte [rdi], cl
	cmp cl, 0
	je .ret
	inc rsi
	inc rdi
	jmp .loop


.ret:
	ret
