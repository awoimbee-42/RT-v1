/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 17:16:07 by awoimbee         ###   ########.fr       */
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
	env.objs_arr = malloc(sizeof(t_obj) * 1);
	env.objs_arr[0].this.sphere.intersect = &hit_distance_sphere;
	env.objs_arr[0].this.sphere.orig = (t_vec3){1.0, 0.5, 5.};
	env.objs_arr[0].this.sphere.radius = 0.3;

	env.objs_arr[0].color.intc = 0xFFFFFF;
	env.objs_arr[0].material = MAT;

	env.objs_nb = 1;
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
