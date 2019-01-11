/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/11 22:02:33 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_id_dist		nearest_obj(const t_env *env, const t_ray ray)
{
	t_id_dist		nearest;
	t_id_dist		tmp;

	nearest.dist = __FLT_MAX__;
	nearest.id = -1;
	tmp.id = -1;
	while (++tmp.id < env->objs_nb)
	{
		tmp.dist = env->objs_arr[tmp.id].this.any
						.distfun(&env->objs_arr[tmp.id].this, ray);
		if (tmp.dist > 0 && tmp.dist < nearest.dist)
		{
			nearest.id = tmp.id;
			nearest.dist = tmp.dist;
		}
	};
	return (nearest);
}

//https://stackoverflow.com/questions/15619830/raytracing-how-to-combine-diffuse-and-specular-color
t_fcolor			spec_diff(const t_env *env, const t_ray hit_norm) //for the moment it's only semi diffraction
{
	float			light_dist;
	t_fcolor		light;

	light = env->bckgrnd_col;
	int i = -1;
	while (++i < env->light_nb)
	{
		light_dist = points_dist(env->light_arr[i].pos, hit_norm.org);
		// printf("light dist: %f	hit_norm.org {x: %f, y: %f, z: %f}\n", light_dist, hit_norm.org.x, hit_norm.org.y, hit_norm.org.z);
		if (light_dist < nearest_obj(env, hit_norm).dist || 1) // this condition is bad, i'm guessing its because of the imprecision of floats
		{
			light = light_add(light, light_drop(env->light_arr[i].intensity, light_dist));
			// printf("light: r=%f g=%f b=%f", )
		}
	}
	return (light);
}

t_fcolor	trace_ray(const t_env *env, const t_ray ray, const int bounce)
{
	t_id_dist		obj;
	t_ray			hit_normal;
	t_fcolor		emit_col;

	if (bounce == 0)
		return (env->bckgrnd_col);
	obj = nearest_obj(env, ray);
	if (obj.id == -1)
		return (env->bckgrnd_col);

	// printf("dist: %f\n", obj.dist);
	hit_normal.org = vec3_add(vec3_multf(ray.dir, obj.dist), ray.org);
	hit_normal.dir = env->objs_arr[obj.id].this.any
						.normfun(&env->objs_arr[obj.id].this, hit_normal.org);
	emit_col = light_mult(spec_diff(env, hit_normal),
							env->objs_arr[obj.id].color);
	return (emit_col);
}

/*
**	raytrace() shoots the rays from the 'camera' onto the 'lens'
**	screen_point are the coordinates of the point of the screen that the ray
**		points to
*/

t_fcolor	launch_ray(const int x, const int y, const t_env *env)
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

	return (trace_ray(env, (t_ray){env->camera.org, screen_point}, 2));
}

void		render(t_env *env)
{
	int				i;
	int				j;
	unsigned long	px_id;
	t_fcolor		px_color;

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
			px_color = launch_ray(j, i, env);
			env->mlx->img.data[px_id++] = srgb(px_color);
		}
	}

	mlx_put_image_to_window(env->mlx->ptr, env->mlx->win, env->mlx->img.ptr, 0, 0);
	mlx_destroy_image(env->mlx->ptr, env->mlx->img.ptr);

}
