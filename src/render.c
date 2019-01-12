/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/12 13:26:36 by awoimbee         ###   ########.fr       */
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
		if (tmp.dist > 0.01 && tmp.dist < nearest.dist)
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
	t_id_dist		near_obj;

	light = env->bckgrnd_col;
	int i = -1;
	while (++i < env->light_nb)
	{
		light_dist = points_dist(env->light_arr[i].pos, hit_norm.org);
		// printf("light dist: %f	hit_norm.org {x: %f, y: %f, z: %f}\n", light_dist, hit_norm.org.x, hit_norm.org.y, hit_norm.org.z);
		near_obj = nearest_obj(env, hit_norm);
		if (light_dist < near_obj.dist) // this condition is bad, i'm guessing its because of the imprecision of floats OR the sphere dist func sucks
		{
			light = light_add(light, light_drop(env->light_arr[i].intensity, light_dist));
			// printf("light: r=%f g=%f b=%f", )
		}
		// else
		// 	printf("light.dist: %7.5f obj.dist: %6.5f hit_norm.org {x: %.5f, y: %.5f, z: %.5f}\n", light_dist, near_obj.dist, hit_norm.org.x, hit_norm.org.y, hit_norm.org.z);
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

void		tone_map(t_fcolor *img, unsigned long px_nb)
{
	unsigned long i;
	float max;

	max = 1;
	i = 0;
	while (i < px_nb)
	{
		img[i] = light_filter(img[i], light_add(img[i], (t_fcolor){1, 1, 1}));
		++i;
	}
}

void		render(t_env *env)
{
	int				u;
	int				v;
	unsigned long	px_id;
	t_fcolor		*img;

	//should mlx calls be protected?
	img = malloc((env->disp.res.x * env->disp.res.y) * sizeof(t_fcolor));
	env->mlx->img.ptr = mlx_new_image(env->mlx->ptr, env->disp.res.x, env->disp.res.y);
	env->mlx->img.data = (int *)mlx_get_data_addr(env->mlx->img.ptr, &env->mlx->img.bpp, &env->mlx->img.line_s, &env->mlx->img.endian);

	px_id = 0;
	v = -1;
	while (++v < env->disp.res.y)
	{
		u = -1;
		while (++u < env->disp.res.x)
		{
			img[px_id++] = launch_ray(u, v, env);
		}
	}
	tone_map(img, px_id);

	px_id = 0;
	v = -1;
	while (++v < env->disp.res.y)
	{
		u = -1;
		while (++u < env->disp.res.x)
		{
			env->mlx->img.data[px_id] = srgb(img[px_id]);
			px_id++;
		}
	}

	free(img);
	mlx_put_image_to_window(env->mlx->ptr, env->mlx->win, env->mlx->img.ptr, 0, 0);
	mlx_destroy_image(env->mlx->ptr, env->mlx->img.ptr);

}
