/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/26 19:09:32 by cpoirier          #+#    #+#             */
/*   Updated: 2019/02/28 19:43:28 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_id_dist			nearest_obj(const t_env *env, const t_ray *ray)
{
	t_id_dist		nearest;
	t_id_dist		tmp;

	nearest.dist = __FLT_MAX__;
	nearest.id = -1;
	tmp.id = -1;
	while (++tmp.id < env->objs_nb)
	{
		tmp.dist = env->objs_arr[tmp.id]
			.distfun(&env->objs_arr[tmp.id].this, ray);
		if (tmp.dist < nearest.dist && tmp.dist > 0.001)
		{
			nearest.id = tmp.id;
			nearest.dist = tmp.dist;
		}
	}
	return (nearest);
}

float				get_specular(const t_vec3 *dir,
	const t_vec3 *light_dir, const float specular, const float light)
{
	float			theta;
	float			is_bright;

	theta = acosf(fmaxf(flt3_dot(dir, light_dir), 0) / (flt3_mod(dir)
				* flt3_mod(light_dir)));
	is_bright = specular * (1 / (theta * theta * theta));
	return (is_bright * light / 10000.);
}

static inline float	lghtobj_dst(const t_env *env, const int i, t_ray *ray[2],
	t_id_dist *near_obj)
{
	float			light_dist;

	ray[0]->org = ray[1]->org;
	ray[0]->dir = env->light_arr[i].pos;
	flt3_sub(&ray[0]->dir, &ray[1]->org);
	light_dist = flt3_mod(&ray[0]->dir);
	flt3_normalize(&ray[0]->dir);
	*near_obj = nearest_obj(env, ray[0]);
	return (light_dist);
}

t_fcolor			fast_diffuse(const t_env *env, t_ray *hit,
	t_obj *obj, t_vec3 *norm)
{
	float			light_dist;
	t_fcolor		light;
	t_id_dist		near_obj;
	int				i;
	t_ray			ray;

	i = -1;
	light = env->bckgrnd_col;
	while (++i < env->light_nb)
	{
		light_dist = lghtobj_dst(env, i, (t_ray*[2]){&ray, hit}, &near_obj);
		if (light_dist < near_obj.dist)
		{
			ray.org = env->light_arr[i].intensity;
			flt3_addf(flt3_add(&light, flt3_multf(
						light_drop(&ray.org, light_dist),
						fmaxf(flt3_dot(norm, &ray.dir), 0.2) * obj->diffuse)),
				get_specular(&hit->dir, &ray.dir, obj->specular,
					flt3_mod(&env->light_arr[i].intensity)));
		}
	}
	return (light);
}
