;void ref_list_sort(t_list **head, int *cmp(void *data1, void *data2)) {
;	[x] if (!head || !*head) {
;	[x] 	return ;
;	[x] }
;
;	[ ] t_list *sorted_head = *head;
;	[ ] *head = (*head)->next;
;	[ ] sorted_head->next = NULL;
;
;	[ ] while (*head) {
;	[ ] 	t_list *to_insert = *head;
;	[ ] 	*head = (*head)->next;
;	[ ] 	to_insert->next = NULL;
;	[ ] 	ref_helper_insert(&sorted_head, to_insert, cmp);
;	[ ] }
;	[ ] *head = sorted_head;
;}

;void ref_helper_insert(t_list **head, t_list *node, int *cmp(void *data, void *data2)) {
;	if (!head) {
;		return ;
;	}
;	if (!*head) {
;		*head = node;
;		return ;
;	}
;	t_list *last = NULL;
;	t_list *cur = *head;
;	while (cur) {
;		if (cmp(cur->data, node->data) > 0) {
;			node->next = cur;
;			if (last) {
;				last->next = node;
;			} else {
;				*head = node;
;			}
;			return ;
;		}
;		last = cur;
;		cur = cur->next;
;	}
;	last->next = node;
;}

section .text
	global ft_list_sort

ft_list_sort:
.loop:
	test rdi, rdi
	jz .return

	mov rdi, [rdi + 8]
	jmp .loop

.return:
	ret
