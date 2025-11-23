;ssize_t ft_write(int fd, const void *buf, size_t count);

section .text
global ft_write
extern __errno_location

;read systel call in linux kernel:
;https://github.com/torvalds/linux/blob/d13f3ac64efb868d09cb2726b1e84929afe90235/fs/read_write.c#L727
;-> return value on error == errno * -1

ft_write:
	mov rax, 1 ; set rax 1 for write syscall
	syscall
	cmp rax, 0
	jge .return
.error:
	neg rax
	push rax
	call __errno_location wrt ..plt ;'wih respect to proceure linkage table' (runtime address determination through lookup)
	pop rcx
	mov [rax], ecx
	mov rax, -1

.return:
	ret
