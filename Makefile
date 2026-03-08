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

bench: fclean
	make test FLAGS="-Ofast"
	cc -Ofast bench.c $(NAME) -o bench
	./test && ./bench

bonus:
	make SRC_FILES="$(BASE_FILES) $(BONUS_FILES)" FLAGS="$(FLAGS) -DBONUS"

$(OBJ_DIR):
	mkdir -p $@

obj/%.o: src/%.s $(OBJ_DIR)
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

.PHONY: all test bonus dep compile re clean fclean val bench

#flags-school: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb ssbd ibrs ibpb stibp ibrs_enhanced tpr_shadow flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid rdseed adx smap clflushopt clwb intel_pt sha_ni xsaveopt xsavec xgetbv1 xsaves split_lock_detect user_shstk avx_vnni dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp hwp_pkg_req hfi vnmi umip pku ospke waitpkg gfni vaes vpclmulqdq tme rdpid movdiri movdir64b fsrm md_clear serialize pconfig arch_lbr ibt flush_l1d arch_capabilities ibpb_exit_to_user
