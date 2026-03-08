;size_t ft_strlen(const char *s);

section .text
	global ft_strlen


ft_strlen:
	xor rax, rax

%macro loop1 0
	%rep 1
		cmp byte [rdi + rax], 0
		je .ret
		inc eax
	%endrep
%endmacro

%macro simd_loop 0
	%rep 2
		;vmovdqu ymm0, [rdi + rax] ; load 32 bytes unalinged
		vmovdqa ymm0, [rdi + rax] ; load 32 bytes alinged
		vpcmpeqb ymm1, ymm0, ymm2 ; each byte is either 0x0 or 0xff, 0xff means termination
		vpmovmskb ecx, ymm1 ; takes the upper bit for each bytes

		test rcx, rcx
		jnz .end_simd
		add eax, 32
	%endrep
	jmp .simd_loop
%endmacro


.loop1:
	loop1
;	jmp .loop1

	; optimization option: No premature alignment.
	; To avoid page errors check how many simd iters can be made instead. Then
	;  align only if needed at a page end.
.alignment: ; 32 byte alignment of rdi + rax

	lea r8, [rdi + rax]
	and r8, 0x1f ; 0b11111 (31); r8 %= 32

	mov rcx, 32
	sub rcx, r8 ; rcx = bytes to alignment

	cmp rcx, 32
	jne .alignment_loop
	jmp .simd

.alignment_loop:
	cmp byte [rdi + rax], 0
	je .ret
	inc eax
	dec rcx
	test ecx, ecx
	jnz .alignment_loop

.simd:
	;mov ecx, 0x0
	;vmovd xmm2, ecx
	;vpbroadcastb ymm2, xmm2 ; ymm2 = 0x00 mask
	vpxor xmm2, xmm2, xmm2

.simd_loop:
	simd_loop

.end_simd:
	bsf ecx, ecx
	add eax, ecx

.ret:
	ret


section .note.GNU-stack noalloc noexec nowrite progbits
