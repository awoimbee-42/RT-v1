/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/26 19:13:58 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*
** t_fcolor			real_diffuse(const t_env *env, const t_ray hit_norm)
**{
**	float			light_dist;
**	t_fcolor		light;
**	t_id_dist		near_obj;
**
**	light = (t_fcolor){0, 0, 0};
**	// shoot rays in a cone shape, then add the fast_diffuse
**		<- need matrix multiplicsation
**		// diffuse_ray(vector + 45 deg on x)
**		// diffuse_ray(vector - 45 deg on x)
**		// diffuse_ray(vector + 45 deg on y)
**		// diffuse_ray(vector - 45 deg on x)
**
**	return (light);
**}
*/

t_fcolor			trace_ray(const t_env *env, const t_ray ray
		, const int bounce)
{
	t_id_dist		obj;
	t_ray			hit_reflect;
	t_fcolor		emit_col;
	t_vec3			norm;

	obj = nearest_obj(env, ray);
	if (obj.id == -1)
		return (env->bckgrnd_col);
	if (!bounce)
		return (env->objs_arr[obj.id].color);
	hit_reflect.org = flt3_add(flt3_multf(ray.dir, obj.dist), ray.org);
	norm = flt3_normalize(env->objs_arr[obj.id].normfun(&env->objs_arr[obj.id]
				.this, hit_reflect.org));
	hit_reflect.dir = get_reflection(ray.dir, norm);
	emit_col = flt3_mult(fast_diffuse(env, hit_reflect, env->objs_arr[obj.id]
				, norm), env->objs_arr[obj.id].color);
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

	screen_point = (t_vec3)
	{
		(2.0 * (x + 0.5) / (float)env->disp.res.x - 1.0) * env->disp.tfov
				* env->disp.aspect_ratio, (1.0 - 2.0 * (y + 0.5)
					/ (float)env->disp.res.y) * env->disp.tfov, 1.0
	};
	apply_camera_rot(env, &screen_point);
	screen_point = flt3_normalize(screen_point);
	return (trace_ray(env, (t_ray){env->camera.org, screen_point}, 1));
}

static int			render_line(void *vthread)
{
	t_thread		*thread;
	unsigned int	*tmp_img;
	int				u;
	int				v;

	thread = (t_thread*)vthread;
	v = thread->line_start;
	tmp_img = &thread->env->sdl->img[v * thread->env->disp.res.x];
	while (v < thread->line_end)
	{
		u = -1;
		while (++u < thread->env->disp.res.x)
			*tmp_img++ = srgb(tone_map(launch_ray(u, v, thread->env)));
		++v;
	}
	return (0);
}

void				render(t_env *env)
{
	int			i;

	i = -1;
	while (++i < THREAD_NB)
		env->threads[i].ptr = SDL_CreateThread(&render_line, ""
				, &env->threads[i]);
	i = -1;
	while (++i < THREAD_NB)
		SDL_WaitThread(env->threads[i].ptr, NULL);
	SDL_UpdateTexture(env->sdl->texture, NULL, env->sdl->img
			, env->disp.res.x * sizeof(int));
	SDL_RenderCopy(env->sdl->renderer, env->sdl->texture, NULL, NULL);
	SDL_RenderPresent(env->sdl->renderer);
}
