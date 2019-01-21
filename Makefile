# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 22:06:19 by marvin            #+#    #+#              #
#    Updated: 2019/01/21 19:39:50 by awoimbee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	rtv1

CC = gcc

CFLAGS	=	-Wall -Wextra -O3 #-Werror -O3 -ffast-math

SRC_NAME =	main.c							\
			init_argv.c						\
			loop.c							\
			render.c						\
			t_obj/dist.c					\
			t_obj/norm.c					\
			operators/flt3_op0.c			\
			operators/flt3_op1.c			\
			operators/special_op.c

SRC_FOLDERS = operators t_obj
################################################################################

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC_PATH =	src
OBJ_PATH =	obj

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

LDLIBS = -lft -lSDL2 -lm

LDFLAGS = -Llibft -LSDL2/lib

CFLAGS += -I./ -I./SDL2/include/ -I./libft

SDL_OPTIONS =	--prefix=$$sdl2dir		\
				--exec-prefix=$$sdl2dir

################################################################################

all : $(NAME)

SDL2/lib/libSDL2.a :
	cd SDL2;															\
	sdl2dir=`pwd`;														\
	mkdir -p build;														\
	cd build;															\
	echo `pwd`;															\
	../sources/configure $(SDL_OPTIONS);								\
	make -j;															\
	make install

libft/libft.a :
	@printf "$(YLW)Making $@...$(EOC)\n"
	@make -s -j -C libft/

$(NAME) : libft/libft.a SDL2/lib/libSDL2.a $(OBJ)
	@printf "$(GRN)Linking $(NAME)...$(EOC)\n"
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_PATH) :
	@mkdir -p $(OBJ_PATH) 2> /dev/null
	@mkdir -p $(addprefix $(OBJ_PATH)/,$(SRC_FOLDERS)) 2> /dev/null
	@printf "$(GRN)Building with \"$(CFLAGS)\":$(EOC)\n"

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c | $(OBJ_PATH)
	@printf "\t$(CC) (...) $@\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

libclean :
	@printf "$(YLW)Cleaning libx...$(EOC)\n"
	@rm -rf SDL2/build SDL2/lib SDL2/share SDL2/bin SDL2/include
	@printf "$(YLW)Cleaning libft...$(EOC)\n"
	@make -s fclean -C libft

objclean :
	@rm -rf $(OBJ_PATH)
	@printf "$(RED)$(OBJ_PATH) removed$(EOC)\n"

outclean :
	@rm -f $(NAME)
	@printf "$(RED)$(NAME) removed$(EOC)\n"

clean	:	libclean	objclean
fclean	:	clean		outclean
re		:	fclean		all
sfclean	:	objclean	outclean
sre		:	sfclean		$(NAME)

.PHONY: all libclean objclean clean re fclean sfclean sre

RED = \033[1;31m
GRN = \033[1;32m
YLW = \033[1;33m
EOC = \033[0m
