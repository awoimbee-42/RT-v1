# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 22:06:19 by marvin            #+#    #+#              #
#    Updated: 2019/01/21 14:28:29 by awoimbee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	rtv1

CC = gcc

CFLAGS	=	-Wall -Wextra -g3 #-Werror -O3 -ffast-math

SRC_NAME =	main.c										\
			init_argv.c									\
			keys_handlers.c								\
			render.c									\
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

LDLIBS = -lft -lmlx -lm -lpthread

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LIBX_FD = ./minilibx_macos
	LDLIBS += -framework OpenGL -framework AppKit
else
	LIBX_FD = ./minilibx_x11
	LDLIBS += -lXext -lX11
	#Don't forget you need libxext-dev & libx11-dev
endif

LDFLAGS = -Llibft -L$(LIBX_FD)
LIBS = libft/libft.a $(LIBX_FD)/libmlx.a

CFLAGS += -I./ -I$(LIBX_FD) -I./libft

################################################################################

all : $(NAME)

$(word 1,$(LIBS)) :
	@printf "$(YLW)Making $@...$(EOC)\n"
	@make -s -j -C libft/

$(word 2,$(LIBS)) :
	@printf "$(YLW)Making $@...$(EOC)\n"
	@make -s -j all -C $(LIBX_FD)

$(NAME) : $(LIBS) $(OBJ)
	@printf "$(GRN)Linking $(NAME)...$(EOC)\n"
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_PATH) :
	@mkdir -p $(OBJ_PATH) 2> /dev/null
	@mkdir -p $(addprefix $(OBJ_PATH)/,$(SRC_FOLDERS)) 2> /dev/null
	@printf "$(GRN)Building with \"$(CFLAGS)\":$(EOC)\n"

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c | $(OBJ_PATH)
	@printf "\t$(CC) (...) $@\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

libclean :
	@printf "$(YLW)Cleaning libx...$(EOC)\n"
	@make -s clean -C $(LIBX_FD)
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
