/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_ope.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/26 19:09:32 by cpoirier          #+#    #+#             */
/*   Updated: 2019/01/26 19:13:43 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_id_dist	nearest_obj(const t_env *env, const t_ray ray)
{
	t_id_dist	nearest;
	t_id_dist	tmp;

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

t_fcolor	get_specular(const t_fcolor light, const t_vec3 dir
		, const t_vec3 light_dir, const float specular)
{
	double		theta;
	double		is_bright;

	theta = acos(fmax(flt3_dot(dir, light_dir), 0));
	is_bright = specular * (1 / (theta * theta * theta));
	is_bright *= is_bright;
	return ((t_vec3){is_bright, is_bright, is_bright});
}

t_fcolor	fast_diffuse(const t_env *env, const t_ray hit
		, const t_obj obj, const t_vec3 norm)
{
	float		light_dist;
	t_fcolor	light;
	t_id_dist	near_obj;
	int			i;
	t_ray		ray;

	light = env->bckgrnd_col;
	i = -1;
	ray.org = hit.org;
	while (++i < env->light_nb)
	{
		ray.dir = flt3_normalize(flt3_sub(env->light_arr[i].pos, hit.org));
		light_dist = flt3_mod(flt3_sub(env->light_arr[i].pos, hit.org));
		near_obj = nearest_obj(env, ray);
		if (light_dist < near_obj.dist)
		{
			light = flt3_add(light, flt3_multf(light_drop(env->light_arr[i].
							intensity, light_dist)
						, fmax(flt3_dot(norm, ray.dir) * obj.diffuse, 0.)));
			light = flt3_add(light, get_specular(light, hit.dir, ray.dir
						, obj.specular));
		}
	}
	return (light);
}
