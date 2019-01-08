/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/08 22:56:30 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"


int		main(int argc, char **argv)
{
	t_mlx	mlx;
	t_env	env;

	init(&env, &mlx);
	read_argv(&env, argv, argc);

	if (!(mlx.win = mlx_new_window(mlx.ptr,
	env.disp.res.x, env.disp.res.y, "RT-V1")))
		error(1);

	//PLACEHOLDER
	env.objects = malloc(sizeof(t_objs) + sizeof(union u_object) * 1);
	env.objects->objs[0].sphere.intersect = &hit_distance_sphere;
	env.objects->objs[0].sphere.orig = (t_vec3){0.5, 0.5, 0.8};
	env.objects->objs[0].sphere.radius = 0.3;
	env.objects->len = 1;
	//####################

	render(&env);
	mlx_loop(mlx.ptr);
}

void	error(int	err_nb)
{
	if (err_nb == 0)
		perror("MiniLibX error: ");
	else  if (err_nb == 1)
		perror("Bad resolution");
	exit(EXIT_FAILURE);
}
