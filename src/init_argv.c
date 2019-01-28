/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:12:01 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 10:14:18 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "../libft/libft.h"

//LOTS OF TODOS IN HERE

void	usage(void)
{
	ft_fprintf(2, "<bold>Usage : ./rtv1 <scene_file></bold>\n");
	exit(EXIT_FAILURE);
}

void	read_argv(t_env *env, char **argv, int argc)
{
	int		i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strcmp(argv[i], "-res") == 0 && i + 2 < argc)
		{
			env->disp.res.x = ft_atoi(argv[++i]);
			env->disp.res.y = ft_atoi(argv[++i]);
			if (env->disp.res.x < 10 || env->disp.res.y < 10
			|| env->disp.res.x < env->disp.res.y)
				msg_exit("Bad resolution", 0);
			env->disp.aspect_ratio = env->disp.res.x / env->disp.res.y;
		}
		else
			usage();
	}
}

void	init_threads(t_env *env)
{
	int i;
	
	i = -1;
	while (++i < THREAD_NB)
	{
		env->threads[i].env = env;
		env->threads[i].line_start = (i * env->disp.res.y) / THREAD_NB;
		env->threads[i].line_end = (((i + 1) * env->disp.res.y) / THREAD_NB);
		env->threads[i].px_start =
			&env->sdl.img[env->threads[i].line_start * env->disp.res.x];
	}
}

void	init_sdl(t_env *env)
{
	t_sdl	*sdl;

	sdl = &env->sdl;
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
		error(SDL_ERR);
	if (!(sdl->win = SDL_CreateWindow( "RT-V1", 0, 0,
		env->disp.res.x, env->disp.res.y, SDL_WINDOW_SHOWN)))
		error(SDL_ERR);
	sdl->renderer = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_ACCELERATED);
	sdl->texture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, env->disp.res.x, env->disp.res.y);
	if (!(sdl->img = malloc(env->disp.res.x * env->disp.res.y * sizeof(int))))
		error(MALLOC_ERR);
}

void	init(t_env *env)
{
	init_sdl(env);
	init_threads(env);
}
