/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/02/26 20:11:11 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_fcolor	trace_ray(const t_env *env, const t_ray *ray, const int bounce)
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

uint32_t	launch_ray(const int x, const int y, const t_env *env)
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

int			thing(int color1, int color2)
{
	// char	*c1;
	// char	*c2;
	// int		ret;
	int		ret;

	// c1 = (char*)&color1;
	// c2 = (char*)&color2;
	// cret = (char*)&ret;
	ret = (((color1 + color2) / 2) & 0xFF0000) + (((color1 + color2) / 2) & 0xFF00) + (((color1 + color2) / 2) & 0xFF) + 0xFF000000;
	// cret[0] = ((int)c1[0] + (int)c2[0]) / 2;
	// cret[1] = ((int)c1[1] + (int)c2[1]) / 2;
	// cret[2] = ((int)c1[2] + (int)c2[2]) / 2;
	return (ret);

}

static int	render_line(void *vthread)
{
	t_thread		*thread;
	t_px_sqr		*tmp_img;
	int				u;
	int				v;

	thread = (t_thread*)vthread;
	v = thread->line_start;
	tmp_img = &thread->env->sdl.big_pxs[((v - 2) / 3 * (thread->env->disp.res.x - 2) / 3) ];
	while (v < thread->line_end)
	{
		u = 1;
		while (u < thread->env->disp.res.x - 3)
		{
			uint32_t	c = launch_ray(u, v, thread->env);
			tmp_img->top_left[0] = c;
			tmp_img->top_left[1] = c;
			tmp_img->top_left[2] = c;

			ft_memcpy(tmp_img->left, &tmp_img->top_left[0], 3 * sizeof(int));
			ft_memcpy(tmp_img->bot_left, &tmp_img->top_left[0], 3 * sizeof(int));
			++tmp_img;
			u += 3;
		}
		// tmp_img -= (thread->env->disp.res.x - u) % 3;
		v += 3;
	}
	return (0);
}

void		render(t_env *env)
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
