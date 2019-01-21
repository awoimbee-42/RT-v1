/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/21 18:48:42 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	placeholder_fill_objs(t_env *env)
{
	env->objs_arr = malloc(sizeof(t_obj) * 4);

	env->objs_arr[0].distfun = &dist_sphere;
	env->objs_arr[0].normfun = &norm_sphere;
	env->objs_arr[0].this.sphere.orig = (t_vec3){0, 0, 5.};
	env->objs_arr[0].this.sphere.radius = 1;
	env->objs_arr[0].color = (t_fcolor){1, 0, 1};

	env->objs_arr[1].distfun = &dist_sphere;
	env->objs_arr[1].normfun = &norm_sphere;
	env->objs_arr[1].this.sphere.orig = (t_vec3){0.8, 2.1, 7.};
	env->objs_arr[1].this.sphere.radius = 2.;
	env->objs_arr[1].color = (t_fcolor){0.2, 0.4, 0.8};

	env->objs_arr[2].distfun = &dist_disk;
	env->objs_arr[2].normfun = &norm_plane;
	env->objs_arr[2].this.disk.orig = (t_vec3){0.8, 2.1, 8.};
	env->objs_arr[2].this.disk.norm = (t_vec3){0, 0, 1};
	env->objs_arr[2].this.disk.radius2 = 16.;
	env->objs_arr[2].color = (t_fcolor){0.8, 0.8, 0.8};

	env->objs_arr[3].distfun = &dist_plane;
	env->objs_arr[3].normfun = &norm_plane;
	env->objs_arr[3].this.plane.orig = (t_vec3){0, -2, 9.};
	env->objs_arr[3].this.plane.norm = (t_vec3){0, 1, 0};
	env->objs_arr[3].color = (t_fcolor){0.5, 0, 0};

	env->objs_nb = 4;
	//####################
	env->light_arr = malloc(sizeof(t_light) * 2);

	env->light_arr[0].pos = (t_vec3){0, -1.2, 5};
	env->light_arr[0].intensity = (t_fcolor){5, 5, 5};

	env->light_arr[1].pos = (t_vec3){0, 100, 0};
	env->light_arr[1].intensity = (t_fcolor){10000, 10000, 10000};

	env->light_nb = 1;
	//######################
}

int		main(int argc, char **argv)
{
	t_sdl	sdl;
	t_env	env;

	init(&env, &sdl);
	read_argv(&env, argv, argc);
	placeholder_fill_objs(&env);
	if (!(sdl.win = SDL_CreateWindow( "RT-V1", 0, 0,
			env.disp.res.x, env.disp.res.y, SDL_WINDOW_SHOWN)))
		error(1);
	sdl.renderer = SDL_CreateRenderer(sdl.win, -1, SDL_RENDERER_ACCELERATED);
	sdl.texture = SDL_CreateTexture(sdl.renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, env.disp.res.x, env.disp.res.y);
	if (!(sdl.img = malloc(env.disp.res.x * env.disp.res.y * sizeof(int))))
		error(2);
	render(&env);
	loop(&env);
	return (0);
}

void	error(int err_nb)
{
	if (err_nb == 0)
		ft_fprintf(2, "SDL_Error: %s\n", SDL_GetError());
	else if (err_nb == 1)
		perror("Bad resolution");
	else if (err_nb == 2)
		perror("Malloc error");
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
