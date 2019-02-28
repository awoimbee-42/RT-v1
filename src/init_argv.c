/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:12:01 by awoimbee          #+#    #+#             */
/*   Updated: 2019/02/28 20:50:36 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "../libft/libft.h"

void	usage(void)
{
	ft_fprintf(2, "<bold>Usage : ./rtv1 file</bold>\n");
	exit(EXIT_FAILURE);
}

void	init_threads(t_env *env)
{
	int i;

	i = -1;
	while (++i < THREAD_NB)
	{
		env->threads[i].env = env;
		env->threads[i].line_start = (i * env->disp.h) / THREAD_NB;
		env->threads[i].line_end = (((i + 1) * env->disp.h) / THREAD_NB);
		env->threads[i].px_start =
			&env->sdl.img[env->threads[i].line_start * env->disp.w];
	}
}

void	init_sdl(t_env *env)
{
	t_sdl		*sdl;

	sdl = &env->sdl;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
		error(SDL_ERR);
	if (!(sdl->win = SDL_CreateWindow("RT-V1", 0, 0,
		env->disp.w, env->disp.h, SDL_WINDOW_SHOWN)))
		error(SDL_ERR);
	sdl->renderer = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_PRESENTVSYNC);
	sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, env->disp.w, env->disp.h);
	if (!(sdl->img = malloc(env->disp.w * env->disp.h * sizeof(int)))
		|| !(sdl->big_pxs = malloc(env->disp.w / 5 * env->disp.h / 5
					* sizeof(*sdl->big_pxs))))
		error(MALLOC_ERR);
}

void	init(t_env *env)
{
	init_sdl(env);
	init_threads(env);
	env->controller = SDL_IsGameController(0) ?
			SDL_GameControllerOpen(0) : NULL;
}
