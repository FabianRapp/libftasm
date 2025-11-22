
;int ft_strcmp(const char *s1, const char *s2);
section .text
	global ft_strcmp

ft_strcmp:
	xor rax, rax
	xor rcx, rcx
.loop:
	mov byte cl, [rsi]
	mov byte al, [rdi]
	cmp byte al, 0; if (!*s1)
	je .ret1; return (*s1 - *s2)
	sub rax, rcx; *s1 - *s2
	jnz .ret ; if (rax = *s1 - *s2) return rax
	inc rdi; s1++
	inc rsi; s1++
	jmp .loop

.ret1:
	sub rax, rcx
.ret:
	ret
