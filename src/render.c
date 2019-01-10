/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/10 18:24:46 by awoimbee         ###   ########.fr       */
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

union u_color	trace_ray(const t_env *env, const t_ray ray, const int bounce)
{
	float			dist[2];
	int				i[2];
	t_ray			hit_normal;
	union u_color	emit_col;

	if (bounce == 0)
		return (env->bckgrnd_col);
	i[0] = -1;
	i[1] = -1;
	dist[1] = __FLT_MAX__;
	while (++i[0] < env->objs_nb)
	{
		dist[0] = env->objs_arr[i[0]].this.any.distfun(&env->objs_arr[i[0]].this, ray);
		if (dist[0] > 0 && dist[0] < dist[1])
		{
			i[1] = i[0];
			dist[1] = dist[0];
		}
	};
	if (env->objs_arr[i[1]].color_filter.intc < 0) // Yes, I want to modify this to make light reflect on light sources !
		emit_col = get_light_col(...);
	else
	{

	}
		
	hit_normal.org = vec3_add(vec3_multf(ray.dir, dist[1]), ray.org);
	hit_normal.dir = env->objs_arr[i[1]].this.any.normfun(&env->objs_arr[i[1]].this, hit_normal.org);

}

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