section .text
	global ft_list_remove_if
	extern free

;typedef struct s_list
;{
;	void *data;
;	struct s_list *next;
;} t_list;


;void ref_list_remove_if(t_list **head, void *data_ref, int (*cmp)(void *data1, void *data), void (*free_fct)(void *data)) {
;	if (!head) {
;		return ;
;	}
;	while (*head && !cmp(data_ref, (*head)->data)) {
;		t_list *old_head = *head;
;		*head = old_head->next;
;		free_fct(old_head->data);
;		free(old_head);
;	}
;	if (!*head) {
;		return ;
;	}
;	t_list *last = *head;
;	t_list *cur = (*head)->next;
;	while (cur) {
;		if (!cmp(data_ref, cur->data)) {
;			last->next = cur->next;
;			free_fct(cur->data);
;			free(cur);
;		} else {
;			last = cur;
;		}
;		cur = last->next;
;	}
;}

ft_list_remove_if:
	push r12
	push r13
	push r14
	push r15
	push rbx

	mov r12, rdi ; r12 = (t_node**)head
	mov r13, rsi; r13 = (void *)data_ref
	mov r14, rdx; r14 = int (*cmp)(void *data1, void *data)
	mov r15, rcx; r15 = void (*free_fct)(void *data)

	test rdi, rdi
	jz .return

.loop1:
;	while (*head && !cmp(data_ref, (*head)->data)) {
;		t_list *old_head = *head;
;		*head = old_head->next;
;		free_fct(old_head->data);
;		free(old_head);
;	}
;	if (!*head) {
;		return ;
;	}

	; rbx(old_head) = *head
	mov rbx, [r12]
	test rbx, rbx
	jz .return

	;cmp(data_ref, data)
	mov rdi, r13
	mov rsi, [rbx]; rsi(data) = *old_head ; old_head->data
	call r14

	;if (cmp(data_ref, data) goto .end_loop1
	test eax, eax
	jnz .end_loop1
	
	; delete data
	mov rdi, [rbx]
	call r15

	push rbx

	; *head = old_head->next
	mov rbx, [rbx + 8]
	mov [r12], rbx

	pop rbx

	; free(old_head)
	mov rdi, rbx
	call free wrt ..plt
	jmp .loop1

.end_loop1:
;	t_list *last = *head;
	mov rbx, [r12]; rbx = last
;	t_list *cur = (*head)->next;
	mov r12, [rbx + 8]; r12 = cur
.loop2:
;	while (cur) {
;		if (!cmp(data_ref, cur->data)) {
;			last->next = cur->next;
;			free_fct(cur->data);
;			free(cur);
;		} else {
;			last = cur;
;		}
;		cur = last->next;
;	}
	test r12, r12
	jz .end_loop2
	mov rdi, r13
	mov rsi, [r12]
	call r14
	test eax, eax
	jnz .skip
.delete:
	; rax = cur->next
	mov rax, [r12 + 8]
	; last->next = cur->next
	mov [rbx + 8], rax

	; delete cur->data
	mov rdi, [r12]
	call r15

	; free (cur)
	mov rdi, r12
	call free wrt ..plt

	jmp .next
.skip:
	; last = cur
	mov rbx, r12
.next:
	; cur = last->next
	mov r12, [rbx + 8]
	jmp .loop2

.end_loop2:

.return:
	pop rbx
	pop r15
	pop r14
	pop r13
	pop r12
	ret
section .note.GNU-stack noalloc noexec nowrite progbits
