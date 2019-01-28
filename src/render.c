/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 16:57:08 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*
**	We compare tmp.dist against 0.01 to prevent the case of
**		the rays hitting objects on their origin point.
*/

static t_id_dist	nearest_obj(const t_env *env, const t_ray *ray)
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

static float		get_specular(const t_vec3 *dir,
					const t_vec3 *light_dir, const float specular)
{
	double		theta;
	double		is_bright;

	theta = acos(fmax(flt3_dot(dir, light_dir), 0) / (flt3_mod(dir)
				* flt3_mod(light_dir)));
	is_bright = specular * (1 / (theta * theta * theta));
	return (is_bright);
}

static t_fcolor		fast_diffuse(const t_env *env, t_ray *hit,
									t_obj *obj, t_vec3 *norm)
{
	float			light_dist;
	t_fcolor		light;
	t_id_dist		near_obj;
	int				i;
	t_ray			ray;
	t_vec3			tmp;
	float			d;

	light = env->bckgrnd_col;
	i = -1;
	ray.org = hit->org;
	while (++i < env->light_nb)
	{
		ray.dir = env->light_arr[i].pos;
		flt3_sub(&ray.dir, &hit->org);
		light_dist = flt3_mod(&ray.dir);
		flt3_normalize(&ray.dir);
		near_obj = nearest_obj(env, &ray);
		if (light_dist < near_obj.dist)
		{
			d = fabs(flt3_dot(norm, &ray.dir) * obj->diffuse);
			tmp = env->light_arr[i].intensity;
			flt3_add(&light, flt3_multf(light_drop(&tmp, light_dist), d));
			flt3_addf(&light, get_specular(&hit->dir, &ray.dir, obj->specular));
		}
	}
	return (light);
}

t_fcolor			trace_ray(const t_env *env, const t_ray *ray,
								const int bounce)
{
	t_id_dist		obj;
	t_ray			hit_reflect;
	t_vec3			norm;

	/*if (bounce == 0)
		return (env->bckgrnd_col);
	*/
	obj = nearest_obj(env, ray);
	if (obj.id == -1)
		return (env->bckgrnd_col);
	if (!bounce)
		return (env->objs_arr[obj.id].color);
	hit_reflect.org = ray->dir;
	flt3_multf(&hit_reflect.org, obj.dist);
	flt3_add(&hit_reflect.org, &ray->org);
	norm = env->objs_arr[obj.id]
			.normfun(&env->objs_arr[obj.id].this, &hit_reflect.org);
	flt3_normalize(&norm);
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
		(2.0 * (x + 0.5) / (float)env->disp.res.x - 1.0)
			* env->disp.tfov * env->disp.aspect_ratio,
		(1.0 - 2.0 * (y + 0.5) / (float)env->disp.res.y) * env->disp.tfov,
		1.0
	};
	apply_camera_rot(env, &screen_point);
	flt3_normalize(&screen_point);
	screen_point = trace_ray(env, &(t_ray){env->camera.org, screen_point}, 1);
	tone_map(&screen_point);
	return (srgb(&screen_point));
}

static int			render_line(void *vthread)
{
	t_thread		*thread;
	uint32_t		*tmp_img;
	int				u;
	int				v;

	thread = (t_thread*)vthread;
	v = thread->line_start;
	tmp_img = &thread->env->sdl.img[v * thread->env->disp.res.x];
	while (v < thread->line_end)
	{
		u = -1;
		while (++u < thread->env->disp.res.x)
			*tmp_img++ = launch_ray(u, v, thread->env);
		++v;
	}
	return (0);
}

void				render(t_env *env)
{
	int			i;

	i = -1;
	while (++i < THREAD_NB)
		env->threads[i].ptr = SDL_CreateThread(&render_line, "",
												&env->threads[i]);
	i = -1;
	while (++i < THREAD_NB)
		SDL_WaitThread(env->threads[i].ptr, NULL);
	SDL_UpdateTexture(env->sdl.texture, NULL, env->sdl.img,
						env->disp.res.x * sizeof(int));
	SDL_RenderCopy(env->sdl.renderer, env->sdl.texture, NULL, NULL);
	SDL_RenderPresent(env->sdl.renderer);
}
