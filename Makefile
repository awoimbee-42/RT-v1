# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 22:06:19 by marvin            #+#    #+#              #
#    Updated: 2019/04/11 16:20:02 by awoimbee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	rtv1

CFLAGS	=	-Wall -Wextra -Ofast -march=native -ftree-vectorize -fstrict-aliasing#-Werror

SRC_NAME =	main.c							\
			init_argv.c						\
			loop.c							\
			t_obj/dist.c					\
			t_obj/norm.c					\
			\
			renderer/calc_light.c				\
			renderer/render_master.c			\
			renderer/launch_ray.c				\
			\
			operators/flt3_op0.c				\
			operators/flt3_op1.c				\
			operators/flt3_op2.c				\
			operators/light.c					\
			operators/camera.c					\
			\
			parser/env_disp_cam.c				\
			parser/lights.c						\
			parser/obj_array.c					\
			parser/obj_individual.c				\
			parser/scene.c						\
			parser/support_funs.c				\
			parser/load_obj.c

SRC_FOLDERS = operators t_obj parser renderer
DEPS_FOLDER =	dependencies
BUILD_FOLDER =	build

################################################################################
###############                  CONSTANTS                       ###############
################################################################################
CC = gcc
LDLIBS = -lft -lm -lSDL2 -lpthread
LDFLAGS = -L./libs/libft -L$(DEPS_FOLDER)/lib #-Wl,-rpath=`pwd`/sdl2/lib
CFLAGS += -MMD -I./ -isystem$(BUILD_FOLDER)/include/ -I./libs/libft

SDL_OPTIONS =	-q									\
				--prefix=$(DEPS_PATH)				\
				--exec-prefix=$(DEPS_PATH)			\
				--bindir=$(BUILD_PATH)/bin			\
				--includedir=$(BUILD_PATH)/include	\
				--datarootdir=$(BUILD_PATH)/share

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
	ifeq ($(PROFILE),gen)
		CFLAGS += -fprofile-generate #using perf is heaps better (fautoprofile something something)
	else ifeq ($(PROFILE),use)
		CFLAGS += -fprofile-use
	endif
else ifeq ($(UNAME_S),Darwin)
	NUMPROC := $(shell sysctl hw.ncpu | awk '{print $$2}')
	PROCESS_PROFDATA := xcrun llvm-profdata merge -sparse default.profraw -o default.profdata
	RM_PROFDATA := rm ./default.profraw ./default.profdata
	ifeq ($(PROFILE),gen)
		CFLAGS += -fprofile-instr-generate
	else ifeq ($(PROFILE),use)
		CFLAGS += -fprofile-instr-use
	endif
endif

REPO_PATH = $(patsubst %/,%,$(dir $(realpath $(firstword $(MAKEFILE_LIST)))))
DEPS_PATH = $(REPO_PATH)/$(DEPS_FOLDER)
BUILD_PATH = $(REPO_PATH)/$(BUILD_FOLDER)
SRC_FOLDER =	src
OBJ_FOLDER = $(BUILD_FOLDER)/obj
DIRS = $(BUILD_FOLDER) $(DEPS_PATH) $(OBJ_FOLDER) $(addprefix $(OBJ_FOLDER)/,$(SRC_FOLDERS))
LSDL2 = $(DEPS_PATH)/lib/libSDL2.a
LFT = libs/libft/libft.a

SRC = $(addprefix $(SRC_FOLDER)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_FOLDER)/,$(SRC_NAME:.c=.o))

################################################################################
#################                  RULES                       #################
################################################################################

all : $(NAME)

############## LIBS ############
$(BUILD_FOLDER)/include/SDL2 : $(LSDL2)
$(LSDL2) : | $(BUILD_FOLDER)
	@printf "$(YLW)Making SDL2...$(EOC)\n"
	@mkdir -p $(BUILD_PATH)/sdl2
	@printf "$(INV)Creating build env...$(EOC)\n"				&&\
		cd $(BUILD_PATH)/sdl2 									&&\
		$(REPO_PATH)/libs/sdl2/configure $(SDL_OPTIONS)			&&\
		printf "$(INV)Building...$(EOC)\n"						&&\
		make -sj 10												&&\
		printf "$(INV)Installing in $(DEPS_FOLDER)...$(EOC)\n"	&&\
		make -s install
	@printf "$(EOC)\n"
$(LFT) :
	@printf "$(YLW)Making libft...$(EOC)\n"
	@make -s -j -C libs/libft/
################################

$(NAME) : $(LFT) $(LSDL2) $(OBJ)
	@printf "$(GRN)Linking $(NAME)...$(EOC)\n"
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

opti : obj_clean
	@printf "Compiling instrumented version...\n"
	@make $(NAME) PROFILE=gen > /dev/null
	@printf "Profiling...\n"
	@./rtv1 ./scenes/scene.rt
	@$(PROCESS_PROFDATA)
	@printf "Cleaning...\n"
	@make obj_clean
	make $(NAME) PROFILE=use
	$(RM_PROFDATA)

$(OBJ_FOLDER)/%.o : $(SRC_FOLDER)/%.c | $(DIRS)
	@printf "\t$(CC) (...) $@\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(DIRS) :
	@mkdir -p $@

# Add rules written in .d files (by gcc -MMD)
# The '-' makes it doesn't care if the file exists or not
-include $(OBJ:.o=.d)

obj_clean :
	@printf "$(RED)Cleaning $(OBJ_FOLDER)$(EOC)\n"
	@rm -rf $(OBJ_FOLDER)

clean :
	@printf "$(RED)Cleaning $(BUILD_FOLDER)$(EOC)\n"
	@rm -rf $(BUILD_FOLDER)
	@printf "$(YLW)Cleaning libft...$(EOC)\n"
	@make -s fclean -C libs/libft

fclean : clean
	@printf "$(YLW)Cleaning libs dependencies...$(EOC)\n"
	@rm -rf $(DEPS_FOLDER)/lib
	@printf "$(RED)Cleaning $(NAME)$(EOC)\n"
	@rm -f $(NAME)

sclean	:	obj_clean
re		:	fclean		all
sre		:	obj_clean		all

.PHONY: all obj_clean clean fclean re sre

RED = \033[1;31m
GRN = \033[1;32m
YLW = \033[1;33m
INV = \033[7m
EOC = \033[0m
