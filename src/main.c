/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 20:05:51 by awoimbee         ###   ########.fr       */
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
	env.objs_arr = malloc(sizeof(t_obj) * 4);

	env.objs_arr[0].this.sphere.intersect = &dist_sphere;
	env.objs_arr[0].this.sphere.orig = (t_vec3){1.0, 0.5, 5.};
	env.objs_arr[0].this.sphere.radius = 0.3;

	env.objs_arr[0].color.intc = 0xFFFFFF;
	env.objs_arr[0].material = MAT;

	env.objs_arr[1].this.sphere.intersect = &dist_sphere;
	env.objs_arr[1].this.sphere.orig = (t_vec3){0.8, 0.1, 7.};
	env.objs_arr[1].this.sphere.radius = 3.;
	env.objs_arr[1].color.intc = 0xFFFF00;
	env.objs_arr[1].material = MAT;

	env.objs_arr[2].this.disk.intersect = &dist_disk;
	env.objs_arr[2].this.disk.orig = (t_vec3){0.8, 0.1, 7.};
	env.objs_arr[2].this.disk.norm = (t_vec3){0, 0, 1};
	env.objs_arr[2].this.disk.radius2 = 25.;
	env.objs_arr[2].color.intc = 0x0000FF;
	env.objs_arr[2].material = MAT;

	env.objs_arr[3].this.plane.intersect = &dist_plane;
	env.objs_arr[3].this.plane.orig = (t_vec3){0.8, 0.1, 9.};
	env.objs_arr[3].this.plane.norm = (t_vec3){0, 0, 1};
	env.objs_arr[3].color.intc = 0xFF0000;
	env.objs_arr[3].material = MAT;

	env.objs_nb = 4;
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
