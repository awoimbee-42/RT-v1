/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/11 01:54:24 by awoimbee         ###   ########.fr       */
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
	env.objs_arr = malloc(sizeof(t_obj) * 3);

	env.objs_arr[0].this.sphere.distfun = &dist_sphere;
	env.objs_arr[0].this.sphere.normfun = &norm_sphere;
	env.objs_arr[0].this.sphere.orig = (t_vec3){0, 0, 5.};
	env.objs_arr[0].this.sphere.radius = 1;
	env.objs_arr[0].color_filter.intc = 0xFF00FF;
	env.objs_arr[0].material = MAT;

	env.objs_arr[1].this.sphere.distfun = &dist_sphere;
	env.objs_arr[1].this.sphere.normfun = &norm_sphere;
	env.objs_arr[1].this.sphere.orig = (t_vec3){0.8, 0.1, 7.};
	env.objs_arr[1].this.sphere.radius = 3.;
	env.objs_arr[1].color_filter.charc = (struct s_charc){-128, -128, -128, 0};
	env.objs_arr[1].material = MAT;

	env.objs_arr[2].this.disk.distfun = &dist_disk;
	env.objs_arr[2].this.disk.normfun = &norm_plane;
	env.objs_arr[2].this.disk.orig = (t_vec3){0.8, 0.1, 17.};
	env.objs_arr[2].this.disk.norm = (t_vec3){0, 0, 1};
	env.objs_arr[2].this.disk.radius2 = 25.;
	env.objs_arr[2].color_filter.intc = 0x0000FF;
	env.objs_arr[2].material = MAT;

	// env.objs_arr[2].this.plane.distfun = &dist_plane;
	// env.objs_arr[2].this.plane.normfun = &norm_plane;
	// env.objs_arr[2].this.plane.orig = (t_vec3){0.8, 0.1, 9.};
	// env.objs_arr[2].this.plane.norm = (t_vec3){0, 0, 1};
	// env.objs_arr[2].color_filter.intc = 0xFF00FF;
	// env.objs_arr[2].material = MAT;

	env.objs_nb = 3;
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
