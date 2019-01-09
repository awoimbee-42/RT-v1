/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:12:01 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 17:47:29 by awoimbee         ###   ########.fr       */
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
		if (ft_strcmp(argv[i], "-res") == 0 && i + 2 < argc)  // need to enforce image width >= image height !
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
		(float)75. / 2. * M_PI / 180.
	};
	env->camera = (t_ray)
	{
		(t_vec3){0., 0., 0.},
		(t_vec3){0., 0., 1.}
	};
	env->mlx = mlx;
	env->keys_presed = 0;
}
