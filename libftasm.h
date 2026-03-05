#pragma once
#include <stdlib.h> //for size_t
#include <stdbool.h>

typedef struct s_list
{
	void *data;
	struct s_list *next;
} t_list;

int ft_atoi_base(char *str, char *base);

void ft_list_push_front(t_list **begin_list, void *data);
int ft_list_size(t_list *head);
void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(void *data1, void *data), void (*free_fct)(void *data));
void ft_list_sort(t_list **head, int (*cmp)(void *data1, void *data2));

ssize_t ft_read(int fd, void *buf, size_t count);
int ft_strcmp(const char *s1, const char *s2);
char *ft_strcpy(char *restrict dst, const char *restrict src);
char *ft_strdup(const char *s);
size_t ft_strlen(const char *s);
ssize_t ft_write(int fd, const void *buf, size_t count);


