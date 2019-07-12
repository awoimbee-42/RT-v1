/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:12:01 by awoimbee          #+#    #+#             */
/*   Updated: 2019/07/11 16:19:12 by awoimbee         ###   ########.fr       */
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
	uint32_t	i;

	env->threads_nb = SDL_GetCPUCount();
	i = -1;
	while (++i < env->threads_nb)
	{
		env->threads[i].env = env;
		env->threads[i].line_start = (i * env->disp.h) / env->threads_nb;
		env->threads[i].line_end = (((i + 1) * env->disp.h) / env->threads_nb);
		env->threads[i].px_start =
			&env->sdl.img[env->threads[i].line_start * env->disp.w];
	}
}

/*
**	SDL_SetWindowMinimumSize Doesn't work on all systems !
*/

void	init_sdl(t_env *env)
{
	t_sdl		*sdl;

	sdl = &env->sdl;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
		error(SDL_ERR);
	if (!(sdl->win = SDL_CreateWindow("RT-V1", 0, 0, env->disp.w, env->disp.h,
				SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))
		|| !(sdl->renderer = SDL_CreateRenderer(sdl->win, -1,
				SDL_RENDERER_PRESENTVSYNC))
		|| !(sdl->texture = SDL_CreateTexture(sdl->renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING, env->disp.w, env->disp.h)))
		error(SDL_ERR);
	SDL_SetWindowMinimumSize(env->sdl.win, 100, 100);
	if (!(sdl->img =
			malloc(env->disp.w * (env->disp.h + NB_PX_SKIP + 500) * sizeof(uint32_t))))
		error(MALLOC_ERR);
}

void	resize(t_env *env)
{
	t_sdl		*sdl;

	sdl = &env->sdl;
	env->stop = 1;
	SDL_WaitThread(env->rndr, NULL);
	SDL_GetRendererOutputSize(env->sdl.renderer, (int*)&env->disp.w, (int*)&env->disp.h);
	env->disp.aspect_ratio = (float)env->disp.w / (float)env->disp.h;
	SDL_DestroyTexture(env->sdl.texture);
	free(env->sdl.img);
	sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, env->disp.w, env->disp.h);
	if (!(sdl->img =
			malloc(env->disp.w * (env->disp.h + NB_PX_SKIP) * sizeof(uint32_t))))
		error(MALLOC_ERR);
	init_threads(env);
	env->rndr = SDL_CreateThread((int (*)(void *))&render, "", env);
}

void	init(t_env *env)
{
	init_sdl(env);
	init_threads(env);
	env->controller = SDL_IsGameController(0) ?
			SDL_GameControllerOpen(0) : NULL;
	env->supersampling_rate = 1;
}
