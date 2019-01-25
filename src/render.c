/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/25 21:01:13 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*
**	We compare tmp.dist against 0.01 to prevent the case of
**		the rays hitting objects on their origin point.
*/

t_id_dist		nearest_obj(const t_env *env, const t_ray ray)
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

//https://stackoverflow.com/questions/15619830/raytracing-how-to-combine-diffuse-and-specular-color
t_fcolor		fast_diffuse(const t_env *env, const t_coords hit)
{
	float			light_dist;
	t_fcolor		light;
	t_id_dist		near_obj;
	int 			i;
	t_ray			ray;

	light = env->bckgrnd_col;
	i = -1;
	ray.org = hit;
	while (++i < env->light_nb)
	{
		ray.dir = flt3_sub(env->light_arr[i].pos, hit);
		ray.dir = flt3_normalize(ray.dir);
		light_dist = flt3_mod(flt3_sub(env->light_arr[i].pos, hit));
		near_obj = nearest_obj(env, ray);
		if (light_dist < near_obj.dist)
			light = flt3_add(light, light_drop(env->light_arr[i].intensity, light_dist));
	}
	flt3_divf(light, env->light_nb);
	return (light);
}

t_fcolor			real_diffuse(const t_env *env, const t_ray hit_norm)
{
	float			light_dist;
	t_fcolor		light;
	t_id_dist		near_obj;

	light = (t_fcolor){0, 0, 0};
	// shoot rays in a cone shape, then add the fast_diffuse <- need matrix multiplicsation
		// diffuse_ray(vector + 45 deg on x)
		// diffuse_ray(vector - 45 deg on x)
		// diffuse_ray(vector + 45 deg on y)
		// diffuse_ray(vector - 45 deg on x)

	return (light);
}

t_fcolor			trace_ray(const t_env *env, const t_ray ray, const int bounce)
{
	t_id_dist		obj;
	t_ray			hit_reflect;
	t_fcolor		emit_col;

	/*if (bounce == 0)
		return (env->bckgrnd_col);
	*/
	obj = nearest_obj(env, ray);
	if (obj.id == -1)
		return (env->bckgrnd_col);
	// printf("dist: %f\n", obj.dist);
	if (!bounce)
		return (env->objs_arr[obj.id].color);
	hit_reflect.org = flt3_add(flt3_multf(ray.dir, obj.dist), ray.org);
	hit_reflect.dir = flt3_normalize(env->objs_arr[obj.id].normfun(&env->objs_arr[obj.id].this, hit_reflect.org));
	//hit_reflect.dir = get_reflection(ray.dir, flt3_normalize(env->objs_arr[obj.id]
						//.normfun(&env->objs_arr[obj.id].this, hit_reflect.org)));  // is there a real need to normalize ?
	emit_col = flt3_mult(fast_diffuse(env, hit_reflect.org),
							env->objs_arr[obj.id].color);
	return (emit_col);
}

/*
**	launch_ray() shoots the rays from the 'camera' onto the 'lens'
**	screen_point are the coordinates of the point of the screen that the ray
**		hits
*/

t_fcolor			launch_ray(const int x, const int y, const t_env *env)
{
	t_vec3			screen_point;

	screen_point = (t_vec3) //ray direction / point on the 'screen' in world coords
	{
		 (2.0 * (x + 0.5) / (float)env->disp.res.x - 1.0) * env->disp.tfov * env->disp.aspect_ratio,
		(1.0 - 2.0 * (y + 0.5) / (float)env->disp.res.y) * env->disp.tfov,
		1.0
	};
	// multiply by world matrix here <<<

	apply_camera_rot(env, &screen_point);

	screen_point = flt3_normalize(screen_point);
	return (trace_ray(env, (t_ray){env->camera.org, screen_point}, 1));
}

void				render(const t_env *env)
{
	int				u;
	int				v;
	unsigned int	*tmp_img;

	tmp_img = env->sdl->img;
	v = -1;
	while (++v < env->disp.res.y)
	{
		u = -1;
		while (++u < env->disp.res.x)
		{
			*tmp_img = srgb(tone_map(launch_ray(u, v, env)));
			++tmp_img;
		}
	}
	SDL_UpdateTexture(env->sdl->texture, NULL, env->sdl->img, env->disp.res.x * sizeof(int));
	SDL_RenderCopy(env->sdl->renderer, env->sdl->texture, NULL, NULL);
	SDL_RenderPresent(env->sdl->renderer);
}
