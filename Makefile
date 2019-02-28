# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 22:06:19 by marvin            #+#    #+#              #
#    Updated: 2019/02/28 19:43:38 by awoimbee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	rtv1

CC = gcc

CFLAGS	=	-Wall -Wextra -g3 #-Ofast -march=native #-Werror

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
			parser/support_funs.c


SRC_FOLDERS = operators t_obj parser renderer
################################################################################

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC_PATH =	src
OBJ_PATH =	obj

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

LDLIBS = -lft -lm -ldl -lSDL2 -lpthread

LDFLAGS = -Llibft -Lsdl2/lib

CFLAGS += -I./ -isystem./sdl2/include/ -I./libft

SDL_OPTIONS =	-q						\
				--prefix=$$sdl2path		\
				--exec-prefix=$$sdl2path

################################################################################

all : $(NAME)

sdl2/lib/libSDL2.a :
	@printf "$(YLW)Making SDL2...$(EOC)\n"
	@cd sdl2;															\
		export CC="gcc -march=native"									\
		sdl2path=`pwd`;													\
		printf "$(INV)Creating build env...$(EOC)\n";					\
		mkdir -p build;													\
		cd build;														\
		../sources/configure $(SDL_OPTIONS);							\
		printf "$(INV)Building...$(EOC)\n";								\
		make -s -j 10;													\
		printf "$(INV)Installing in ./SDL2...$(EOC)\n";					\
		make -s install

libft/libft.a :
	@printf "$(YLW)Making libft...$(EOC)\n"
	@make -s -j -C libft/

$(NAME) : libft/libft.a sdl2/lib/libSDL2.a $(OBJ) rtv1.h
	@printf "$(GRN)Linking $(NAME)...$(EOC)\n"
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_PATH) : sdl2/lib/libSDL2.a
	@mkdir -p $(OBJ_PATH) 2> /dev/null
	@mkdir -p $(addprefix $(OBJ_PATH)/,$(SRC_FOLDERS)) 2> /dev/null
	@printf "$(GRN)Building with \"$(CFLAGS)\":$(EOC)\n"

$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c | $(OBJ_PATH)
	@printf "\t$(CC) (...) $@\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

libclean :
	@printf "$(YLW)Cleaning SDL2...$(EOC)\n"
	@rm -rf sdl2/build
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
	@rm -rf sdl2/lib sdl2/share sdl2/bin sdl2/include
re		:	fclean
	make -sj all
sfclean	:	objclean	outclean
sre		:	sfclean
	make -sj $(NAME)

.PHONY: all libclean objclean outclean clean fclean re sfclean sre

RED = \033[1;31m
GRN = \033[1;32m
YLW = \033[1;33m
INV = \033[7m
EOC = \033[0m
