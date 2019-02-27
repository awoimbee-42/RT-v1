/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/02/27 19:47:39 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static t_fcolor	trace_ray(const t_env *env, const t_ray *ray, const int bounce)
{
	t_id_dist		obj;
	t_ray			hit_reflect;
	t_vec3			norm;

	obj = nearest_obj(env, ray);
	if (obj.id == -1)
		return (env->bckgrnd_col);
	if (!bounce)
		return (env->objs_arr[obj.id].color);
	hit_reflect.org = ray->dir;
	flt3_add(flt3_multf(&hit_reflect.org, obj.dist), &ray->org);
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

static uint32_t	launch_ray(const int x, const int y, const t_env *env)
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

static int		render_line(void *vthread)
{
	t_thread		*thread;
	t_px_sqr		*tmp_img;
	int				u;
	int				v;

	thread = (t_thread*)vthread;
	v = thread->line_start;
	tmp_img = &thread->env->sdl.big_pxs[((v - 3) / 5 * (thread->env->disp.res.x - 5) / 5) ];
	while (v < thread->line_end)
	{
		u = 3;
		while (u < thread->env->disp.res.x - 3)
		{
			uint32_t	c = launch_ray(u, v, thread->env);
			// fprintf(stderr, "v: %d\t\tu: %d\n", v, u);
			ft_mem32set(tmp_img->top_left, c, 5);
			ft_mem32set(tmp_img->left, c, 5);
			ft_mem32set(tmp_img->bot_left, c, 5);
			++tmp_img;
			u += 5;
		}
		// tmp_img -= (thread->env->disp.res.x - u) % 3;
		v += 5;
	}
	return (0);
}

void			render(t_env *env)
{
	int			i;

	i = -1;
	while (++i < THREAD_NB)
		env->threads[i].ptr = SDL_CreateThread(&render_line, "",
												&env->threads[i]);
	i = -1;
	while (++i < THREAD_NB)
		SDL_WaitThread(env->threads[i].ptr, NULL);
	// ft_memmove(env->sdl.img + 1, env->sdl.img, env->disp.res.x * sizeof(int));
	// ft_memmove(env->sdl.img + 1, env->sdl.img, 1);
	SDL_UpdateTexture(env->sdl.texture, NULL, env->sdl.img,
						env->disp.res.x * sizeof(int));
	SDL_RenderCopy(env->sdl.renderer, env->sdl.texture, NULL, NULL);
	SDL_RenderPresent(env->sdl.renderer);
}
