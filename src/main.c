/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/08/24 00:41:57 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		main(int argc, char **argv)
{
	t_env	*env;

	if (argc != 2)
		usage();
	if (!(env = malloc(sizeof(t_env)
				+ (SDL_GetCPUCount() + 2) * sizeof(t_thread))))
		error(MALLOC_ERR);
	parse_scene(env, argv[1]);
	init(env);
	loop(env, env->controller);
}

void	error(int err_nb)
{
	if (err_nb == SDL_ERR)
		ft_fprintf(2, "SDL_Error: %s\n", SDL_GetError());
	else if (err_nb == MALLOC_ERR)
		perror("Malloc error");
	else if (err_nb != 0)
		ft_fprintf(2, "Unknown error number: %d\n", err_nb);
	exit(EXIT_FAILURE);
}

/*
**	All the crashes in here are caused by
**	/usr/lib/libnvidia-glcore.so.430.40
**
**	#0  0x00007ffff7b96c90 in free () from /usr/lib/libc.so.6
**	#1  0x00007ffff55d1a34 in ?? () from /usr/lib/libnvidia-glcore.so.430.40
**	#2  0x00007ffff5209bed in ?? () from /usr/lib/libnvidia-glcore.so.430.40
**	#3  0x00007ffff56c1d72 in ?? () from /usr/lib/libnvidia-glcore.so.430.40
**	#4  0x00007ffff53226bf in ?? () from /usr/lib/libnvidia-glcore.so.430.40
**	#5  0x00007ffff52e4623 in ?? () from /usr/lib/libnvidia-glcore.so.430.40
**	#6  0x00007ffff7d3ce39 in ?? () from /usr/lib/libSDL2-2.0.so.0
**	#7  0x00007ffff7d3925d in ?? () from /usr/lib/libSDL2-2.0.so.0
**	#8  0x00005555555565a3 in exit_cleanly (env=0x555555566260) at src/main.c:49
*/

void	exit_cleanly(t_env *env)
{
	env->stop = 1;
	SDL_WaitThread(env->rndr, NULL);
	env->controller ? SDL_GameControllerClose(env->controller) : (void)0;
	SDL_DestroyTexture(env->sdl.texture);
	SDL_DestroyRenderer(env->sdl.renderer);
	free(env->sdl.img);
	free(env->light_arr);
	free(env->objs_arr);
	SDL_DestroyWindow(env->sdl.win);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}
