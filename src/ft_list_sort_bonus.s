section .text
global ft_list_sort
extern ft_list_size

;void ref2_list_sort(t_list **head, int (*cmp)(void *data1, void *data2)) {
;	if (!head || !*head) {
;		return ;
;	}
;	int len = ft_list_size(*head);
;	while (--len) {
;		int inner_iter_count = len;
;		t_list *cur = *head;
;		t_list *next = cur->next;
;		while (inner_iter_count--) {
;			if (cmp(cur->data, next->data) > 0) {
;				cur->data = (void*)((uintptr_t)cur->data ^ (uintptr_t)next->data);
;				next->data = (void*)((uintptr_t)cur->data ^ (uintptr_t)next->data);
;				cur->data = (void*)((uintptr_t)cur->data ^ (uintptr_t)next->data);
;			}
;			cur = next;
;			next = cur->next;
;		}
;	}
;}

section .text
	global ft_list_sort

ft_list_sort:
	; if (!head) return
	test rdi, rdi
	jz .return1

	push r12
	push r13
	push r14
	push r15
	sub rsp, 16

	mov r12, rsi ; r12 = cmp

	mov rdi, [rdi] ; prep rdi as ft_list_size args = *head
	mov r14, rdi ; r14 = *head
	test r14, r14
	jz .return2

	call ft_list_size
	mov r13, rax ; r13 = unsorted_len

.outer_loop: ; while (true) {
	dec r13
	test r13, r13 ; if(!unsorted_len)
	jz .return2 ; return

	mov r15, r13 ; r15 = inner_loop_iters = unsorted_len

	mov [rsp], r14 ; [rsp] = cur

	mov rax, r14
	mov rax, [rax + 8]
	mov [rsp + 8], rax ; [rsp + 8] = next;

.inner_loop: ; do {

	mov rax, [rsp]
	mov rdi, [rax] ; rdi = cur->data
	mov rax, [rsp+ 8]
	mov rsi, [rax] ; rsi = next->data

	push rdi
	push rsi
	; cmp(cur->data, next->data)
	call r12

	pop rsi
	pop rdi

	test eax, eax
	jle .next_iter; if <= 0 jmp

	mov rcx, [rsp]
	mov [rcx], rsi

	mov rcx, [rsp + 8]
	mov [rcx], rdi


.next_iter:
;	cur = next;
	mov rax, [rsp + 8] ; rax = [rsp + 8] = next
	mov [rsp], rax ; cur = [rsp] = next

;	next = cur->next;
	mov rax, [rsp] ; rax = cur
	mov rax, [rax + 8] ; rax = [cur + 8] = cur->next
	mov [rsp + 8], rax ; next = [rsp + 8] = rax

	dec r15 ; inner_loop_iters--
	test r15, r15; } while (inner_loop_iters)
	jnz .inner_loop

	jmp .outer_loop ; }


.return1:
	ret

.return2:
	add rsp, 16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
