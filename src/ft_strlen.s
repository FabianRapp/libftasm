;size_t ft_strlen(const char *s);

section .text
	global ft_strlen


ft_strlen:
	xor rax, rax

%macro loop1 0
	%rep 32
		cmp byte [rdi + rax], 0
		je .ret
		inc rax
	%endrep
%endmacro


; do basic checks with an unrolled loop for small strings so there is little overhead
.loop1:
	loop1
	jmp .loop1
	; todo: aligment for simd
	; todo: simd

.ret:
	ret



;ft_strlen:
;	xor rax, rax
;.loop:
;	cmp byte [rdi], 0
;	je .ret
;	inc rax
;	inc rdi
;	jmp .loop
;.ret:
;	ret
section .note.GNU-stack noalloc noexec nowrite progbits
