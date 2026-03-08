;size_t ft_strlen(const char *s);

;todo:
; rep prefix, usefull here?

section .text
	global ft_strlen


ft_strlen:
	xor rax, rax

%macro loop1 0
	%rep 32
		cmp byte [rdi + rax], 0
		je .ret
		inc eax
	%endrep
%endmacro


; do basic checks with an unrolled loop for small strings so there is little overhead
.loop1:
	loop1
;	jmp .loop1


.alignment: ; 32 byte alignment of rdi + rax
	lea r8, [rdi + rax]
	and r8, 0x1f ; 0b11111 (31)

	mov rcx, 32
	sub rcx, r8

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
	mov ecx, 0x0
	vmovd xmm2, ecx
	vpbroadcastb ymm2, xmm2 ; ymm2 = 0x00 mask

.simd_loop:
	vmovdqu ymm0, [rdi + rax] ; load 32 bytes
	vpcmpeqb ymm1, ymm0, ymm2 ; each byte is either 0x0 or 0xff, 0xff means termination
	vpmovmskb ecx, ymm1 ; takes the upper bit for each bytes

	test rcx, rcx
	jnz .end_simd
	add eax, 32
	jmp .simd_loop

.end_simd:
	bsf ecx, ecx
	add eax, ecx

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


;.alignment:
;	push rax
;	;vmovdqu ymm0, [rdi + rax] ; ;load 128 byte
;	add rax, rdi ; div lower 64 bits
;	xor rdx, rdx ; div upper 64 bits
;	mov rcx, 32
;	div rcx ; rdx = (rdi + cur_len) % 32
;	; rdx now holds the distance to the alignment
;	pop rax
;.alignment_loop:
;	cmp byte [rdi + rax], 0
;	je .ret
;	inc rax
;	dec rdx
;	test rdx, rdx
;	jnz .alignment_loop


section .note.GNU-stack noalloc noexec nowrite progbits
