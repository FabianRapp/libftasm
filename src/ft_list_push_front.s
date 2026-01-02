
;void ft_list_push_front(t_list **begin_list, void *data);
extern malloc

section .text
	global ft_list_push_front

ft_list_push_front:
	test rdi, rdi
	jz .return

	push rdi
	push rsi

	;new = malloc(sizeof t_node)
	mov rdi, 16
	call malloc wrt ..plt
	pop rsi
	pop rdi

	test rax, rax
	jz .return

	mov qword [rax], 0
	mov qword [rax + 8], 0

	mov [rax], rsi ;new->data = data

	mov rcx, [rdi]; old_head = *head
	mov [rdi], rax ; *head = new

	mov [rax + 8], rcx; new->next = old_head



.return:
	ret
