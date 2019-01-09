/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 19:38:50 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

// t_vec3			cast_ray(const t_env *env, const t_vec3 *ray_dir)
// {
// 	float		frwrd_; // je sais pas comme l'appeler
// 	int			step;
// 	t_vec3		pos;

// 	frwrd_ = NEAR_PLANE; // as far as i know, objects cannot be closer than this
// 	step = 0;
// 	while (step++ < 500 )//env->max_step)
// 	{
// 		// c'est pas opti de recalculer a partir de 0 a chaque fois mais avec l'imprecision des floats c'est fifficile de faire autrement
// 		pos = *ray_dir;
// 		(void)vec3_add(vec3_multf(&pos, frwrd_), &env->camera.org);


// 	}
// }

/*
**	raytrace() shoots the rays from the 'camera' onto the 'lens'
**	screen_point are the coordinates of the point of the screen that the ray
**		points to
*/

union u_color	raytrace(const int x, const int y, const t_env *env)
{
	t_vec3			screen_point;

	screen_point = (t_vec3) //ray direction / point on the 'screen' in world coords
	{
		(2.0 * (x + 0.5) / (float)env->disp.res.x - 1.0) * tan(env->disp.fov) * env->disp.aspect_ratio,
		(1.0 - 2.0 * (y + 0.5) / (float)env->disp.res.y) * tan(env->disp.fov),
		1.0
	};
	// multiply by world matrix here <<<
	screen_point = vec3_normalize(screen_point);

	// ft_printf("screen_point: .x=%f\t.y=%f\t.z=%f\n", screen_point.x, screen_point.y, screen_point.z);

	int i = 0;
	float distance;
	while (i < env->objs_nb)
	{
		distance = env->objs_arr[i].this.sphere.intersect(&env->objs_arr[i].this, (t_ray){env->camera.org, screen_point});
		if (distance > 0)
			return (env->objs_arr[i].color);
		++i;
	}
	return ((t_color){.intc = 0});
	// should the intersection function also return the normal ?
	// should we pack another function pointer inside the object struct ?

	// normal = point at hit - center of sphere

	// intersec_point = cast_ray(env, &screen_point);
}

void		render(t_env *env)
{
	int				i;
	int				j;
	unsigned long	px_id;
	union u_color	color;

	//should mlx calls be protected?
	env->mlx->img.ptr = mlx_new_image(env->mlx->ptr, env->disp.res.x, env->disp.res.y);
	env->mlx->img.data = (int *)mlx_get_data_addr(env->mlx->img.ptr, &env->mlx->img.bpp, &env->mlx->img.line_s, &env->mlx->img.endian);

	px_id = 0;
	i = -1;
	while (++i < env->disp.res.y)
	{
		j = -1;
		while (++j < env->disp.res.x)
		{
			color = raytrace(j, i, env);
			env->mlx->img.data[px_id++] = color.intc;
		}
	}

	mlx_put_image_to_window(env->mlx->ptr, env->mlx->win, env->mlx->img.ptr, 0, 0);
	mlx_destroy_image(env->mlx->ptr, env->mlx->img.ptr);

}