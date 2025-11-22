;size_t ft_strlen(const char *s);

section .text
	global ft_strlen

ft_strlen:
	xor rax, rax
.loop:
	cmp byte [rdi], 0
	je .ret
	inc rax
	inc rdi
	jmp .loop
.ret:
	ret

