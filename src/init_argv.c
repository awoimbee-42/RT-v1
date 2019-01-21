/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:12:01 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/21 13:55:56 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

//LOTS OF TODOS IN HERE

void	usage(void)
{
	ft_fprintf(2,
		"<bold>Usage : ./rtv1 [-res x y]</bold>\n"
		"\tother stuff\n"
		"\t\tmore stuff\n");
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
				error(1);
			env->disp.aspect_ratio = env->disp.res.x / env->disp.res.y;
		}
		else
			usage();
	}
}

void	init(t_env *env, t_mlx *mlx)
{
	if (!(mlx->ptr = mlx_init()))
		error(0);
	env->disp = (t_disp)
	{
		.res = (t_int2){WIN_W, WIN_H},
		.aspect_ratio = (float)WIN_W / WIN_H,
		.tfov = tan(75. / 2. * M_PI / 180.)
	};
	env->camera = (t_ray)
	{
		(t_vec3){1., 1., 0.},
		(t_vec3){-0.6, 0., 1.}
	};
	env->bckgrnd_col = (t_fcolor){0.05, 0.05, 0.05};
	env->mlx = mlx;
	env->keys_pressed = 0;
}
