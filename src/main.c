/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/26 18:03:44 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		main(int argc, char **argv)
{
	t_sdl	sdl;
	t_env	env;
	int		i;

	init(&env, &sdl);
	argc != 2 ? usage() : (void)0;
	parse_scene(&env, argv[1]);
	if (!(sdl.win = SDL_CreateWindow("RT-V1", 0, 0, env.disp.res.x, env.disp
					.res.y, SDL_WINDOW_SHOWN)))
		error(SDL_ERR);
	sdl.renderer = SDL_CreateRenderer(sdl.win, -1, SDL_RENDERER_ACCELERATED);
	sdl.texture = SDL_CreateTexture(sdl.renderer, SDL_PIXELFORMAT_ARGB8888
			, SDL_TEXTUREACCESS_STREAMING, env.disp.res.x, env.disp.res.y);
	if (!(sdl.img = malloc(env.disp.res.x * env.disp.res.y * sizeof(int))))
		error(MALLOC_ERR);
	i = -1;
	while (++i < THREAD_NB)
	{
		env.threads[i].env = &env;
		env.threads[i].line_start = (i * env.disp.res.y) / THREAD_NB;
		env.threads[i].line_end = (((i + 1) * env.disp.res.y) / THREAD_NB);
	}
	render(&env);
	loop(&env);
	return (0);
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

void	exit_cleanly(t_env *env)
{
	SDL_DestroyTexture(env->sdl->texture);
	SDL_DestroyRenderer(env->sdl->renderer);
	free(env->sdl->img);
	free(env->light_arr);
	free(env->objs_arr);
	SDL_DestroyWindow(env->sdl->win);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}
