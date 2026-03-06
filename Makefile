FLAGS = -O0 -g
BASE_FILES = \
	ft_read.s \
	ft_strcmp.s \
	ft_strcpy.s \
	ft_strdup.s \
	ft_strlen.s \
	ft_write.s
BONUS_FILES = \
	ft_atoi_base_bonus.s \
	ft_list_push_front_bonus.s \
	ft_list_remove_if_bonus.s \
	ft_list_size_bonus.s \
	ft_list_sort_bonus.s

SRC_FILES := $(BASE_FILES)

SRC = $(addprefix src/, $(SRC_FILES))

OBJ_DIR := obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.s=.o))

NAME := libasm.a

all: $(NAME) test

$(NAME): $(OBJ_DIR) $(SRC) $(OBJ)
	ar rcs $(NAME) $(OBJ)

test: main.c $(NAME)
	cc $(FLAGS) main.c $(NAME) -o test

bonus:
	make SRC_FILES="$(BASE_FILES) $(BONUS_FILES)" FLAGS="$(FLAGS) -DBONUS"

$(OBJ_DIR):
	mkdir -p $@

obj/%.o: src/%.r
	nasm -f elf64 -g -F dwarf $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f FILE1

fclean: clean
	rm -f $(NAME) a.out test

re: fclean all

dep:
	brew install nasm

val: re
	valgrind --leak-check=full --show-leak-kinds=all -s --track-origins=yes ./a.out

.PHONY: all test bonus dep compile re clean fclean val
