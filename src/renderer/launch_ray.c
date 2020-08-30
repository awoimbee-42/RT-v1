/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_ray.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arthur <arthur@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 19:24:45 by awoimbee          #+#    #+#             */
/*   Updated: 2020/08/30 23:15:45 by arthur           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_fcolor	ray_intsect(const t_env *env, const t_ray *ray, int bounce)
{
	t_id_dist		obj;
	t_ray			hit_reflect;
	t_vec3			norm;

	obj = nearest_obj(env, ray);
	if (obj.id == -1)
		return (env->bckgrnd_col);
	if (!bounce)
		return (env->objs_arr[obj.id].color);
	obj.dist = obj.dist - obj.dist * 0.001;
	hit_reflect.org = ray->dir;
	flt3_add(flt3_multf(&hit_reflect.org, obj.dist), &ray->org);
	norm = env->objs_arr[obj.id]
		.normfun(&env->objs_arr[obj.id].this, &hit_reflect.org);
	hit_reflect.dir = get_reflection(ray->dir, norm);
	hit_reflect.dir = fast_diffuse(env, &hit_reflect,
			&env->objs_arr[obj.id], &norm);
	flt3_mult(&hit_reflect.dir,
		&env->objs_arr[obj.id].color);
	return (hit_reflect.dir);
}

/*
**	launch_ray() shoots the rays from the 'camera' onto the 'lens'
**	screen_point are the coordinates of the point of the screen that the ray
**		hits == the ray direction
*/

uint32_t		launch_ray(const int x, const int y, const t_env *env)
{
	t_vec3			screen_point;

	screen_point = (t_vec3)
	{
		(2.0 * (x + 0.5) / (env->disp.w) - 1.0)
			* env->disp.tfov * env->disp.aspect_ratio,
		(1.0 - 2.0 * (y + 0.5) / (env->disp.h)) * env->disp.tfov,
		1.0
	};
	apply_camera_rot(env, &screen_point);
	flt3_normalize(&screen_point);
	screen_point = ray_intsect(env, &(t_ray){env->camera.org, screen_point}, 1);
	tone_map(&screen_point);
	return (srgb(&screen_point));
}

static inline t_flt3	flt3_add_(t_flt3 a, t_flt3 b)
{
	t_flt3 res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return (res);
}

uint32_t		launch_ray_supersample(const int x, const int y, const t_env *env, int sampling)
{
	t_vec3			screen_point;
	t_fcolor		px_col;
	t_flt2			px_coord;
	float			px_step;

	px_col = (t_fcolor){0, 0, 0};
	px_step = 1. / (sampling);
	px_coord = (t_flt2){x * sampling , y * sampling};
	for (int _i = 0; _i < sampling; ++_i, px_coord.x += px_step)
	{
		for (int _j = 0; _j < sampling; ++_j, px_coord.y += px_step)
		{
			screen_point = (t_vec3)
			{
				(2.0 * (px_coord.x + 0.5) / (env->disp.w * sampling) - 1.0)
					* env->disp.tfov * env->disp.aspect_ratio,
				(1.0 - 2.0 * (px_coord.y + 0.5) / (env->disp.h * sampling)) * env->disp.tfov,
				1.0
			};
			apply_camera_rot(env, &screen_point);
			flt3_normalize(&screen_point);
			px_col = flt3_add_(px_col, ray_intsect(env, &(t_ray){env->camera.org, screen_point}, 1));
		}
	}
	flt3_divf(&px_col, sampling * sampling);
	tone_map(&px_col);
	return (srgb(&px_col));
}
