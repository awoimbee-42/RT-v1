/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/24 15:34:36 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		main(int argc, char **argv)
{
	t_sdl	sdl;
	t_env	env;

	init(&env, &sdl);
	read_argv(&env, argv, argc);
	parse_scene(&env, "scene.rt");
	if (!(sdl.win = SDL_CreateWindow( "RT-V1", 0, 0,
			env.disp.res.x, env.disp.res.y, SDL_WINDOW_SHOWN)))
		error(SDL_ERR);
	sdl.renderer = SDL_CreateRenderer(sdl.win, -1, SDL_RENDERER_ACCELERATED);
	sdl.texture = SDL_CreateTexture(sdl.renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, env.disp.res.x, env.disp.res.y);
	if (!(sdl.img = malloc(env.disp.res.x * env.disp.res.y * sizeof(int))))
		error(MALLOC_ERR);
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

void			exit_cleanly(t_env *env)
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
