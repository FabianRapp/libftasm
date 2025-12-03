FLAGS = -O0 -g
SRC_FILES = \
	ft_atoi_base.s \
	ft_list_push.s \
	ft_list_remove_if.s \
	ft_list_size.s \
	ft_list_sort.s \
	ft_read.s \
	ft_strcmp.s \
	ft_strcpy.s \
	ft_strdup.s \
	ft_strlen.s \
	ft_write.s \

SRC = $(addprefix src/, $(SRC_FILES))

OBJ_DIR := obj
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.s=.o))

LIB := libftasm.a


compile: $(LIB) main.c
	cc $(FLAGS) main.c $(LIB)

$(LIB): $(OBJ_DIR) $(SRC) $(OBJ)
	ar rcs $(LIB) $(OBJ)

$(OBJ_DIR):
	mkdir -p $@

obj/%.o: src/%.s
	nasm -f elf64 -g -F dwarf $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f FILE1

fclean: clean
	rm -f $(LIB) a.out

re: fclean compile

dep:
	brew install nasm

.PHONY: dep compile re clean fclean
