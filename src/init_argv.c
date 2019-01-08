/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:12:01 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/08 22:47:13 by awoimbee         ###   ########.fr       */
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

	i = 1;
	while (++i < argc)
	{
		if (ft_strcmp(argv[i], "-res") == 0 && i + 2 < argc)
		{
			(env->disp.res.x = ft_atoi(argv[++i])) < 10 ?
			error(1) : 0;
			(env->disp.res.y = ft_atoi(argv[++i])) < 10 ?
			error(1) : 0;
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
		(t_int2){RES_H, RES_V},
		(float)RES_H / RES_V,
		(t_vec2){M_PI, M_PI * (float)RES_H / RES_V}
	};
	env->camera = (t_ray)
	{
		(t_vec3){0., 0., 0.},
		(t_vec3){0., 0., 1.}
	};
	env->mlx = mlx;
	env->keys_presed = 0;
}
