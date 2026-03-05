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
	jz .return
	mov rdi, [rdi]
	test rdi, rdi
	jz .return

	mov rdx, rsi ; rdx = cmp

	call ft_list_size

.outer_loop: ; while (len > 1)
	dec rax
	test rax, rax
	jz .return

	mov rdx, rax ; inner_loop_iters = len
.inner_loop:

;todo: cmp call can alter regs
	; cmp(cur, next); todo: cmp(cur->data, next->data)
	mov rsi, [rdi + 8]
	call rdx


	dec rdx
	test rdx, rdx

	jnz .inner_loop
	jmp .outer_loop


.return:
	ret
