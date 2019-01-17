# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 22:06:19 by marvin            #+#    #+#              #
#    Updated: 2019/01/17 02:10:41 by awoimbee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	rtv1

CC = gcc

CFLAGS	=	-Wall -Wextra -Ofast #-Werror -O3 -ffast-math

SRC_PATH =	src
OBJ_PATH =	obj

SRC_NAME =	main.c										\
			init_argv.c									\
			render.c									\
			dist_functions.c	norm_functions.c		\
			vec3_op0.c			vec3_op1.c				\
			color_op.c

OBJ_NAME = $(SRC_NAME:.c=.o)

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

CPPFLAGS = -I./ -I$(LIBX_FD) -I./libft

LIBS = libft/libft.a $(LIBX_FD)/libmlx.a

all : $(NAME)

libft/libft.a :
	@printf "$(RED)Making libft...$(EOC)\n"
	@make -s -j -C libft/

$(LIBX_FD)/libmlx.a :
	@printf "$(RED)Making libx...$(EOC)\n"
	@make -s -j all -C $(LIBX_FD)

$(NAME) : $(LIBS) $(OBJ)
	@printf "$(RED)Linking $(NAME)...$(EOC)\n"
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@printf "$(GRN)Building $@ with \"$(CFLAGS) $(CPPFLAGS)\"...$(EOC)\n"
	@mkdir -p $(OBJ_PATH) 2> /dev/null
	@$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

libclean :
	@printf "$(RED)Cleaning libx...$(EOC)\n"
	@make -s clean -C $(LIBX_FD)
	@printf "$(RED)Cleaning libft...$(EOC)\n"
	@make -s fclean -C libft

objclean :
	@printf "$(RED)Cleaning objects...$(EOC)\n\trm -rf $(OBJ_PATH)\n"
	@rm -rf $(OBJ_PATH)

clean : libclean objclean

outclean :
	@printf "$(RED)Cleaning $(NAME)...$(EOC)\n\trm -f $(NAME)\n"
	@rm -f $(NAME)

fclean : clean outclean
re : fclean all
sfclean : objclean outclean
sre: sfclean $(NAME)

.PHONY: all libclean objclean clean re fclean sfclean sre

RED = \033[1;31m
GRN = \033[1;32m
EOC = \033[0m
