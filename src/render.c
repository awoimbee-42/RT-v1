/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 16:12:32 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3			cast_ray(const t_env *env, const t_vec3 *ray_dir)
{
	float		frwrd_; // je sais pas comme l'appeler
	int			step;
	t_vec3		pos;

	frwrd_ = NEAR_PLANE; // as far as i know, objects cannot be closer than this
	step = 0;
	while (step++ < 500 )//env->max_step)
	{
		// c'est pas opti de recalculer a partir de 0 a chaque fois mais avec l'imprecision des floats c'est fifficile de faire autrement
		pos = *ray_dir;
		(void)vec3_add(vec3_multf(&pos, frwrd_), &env->camera.org);


	}
}

/*
**	raytrace() shoots the rays from the 'camera' onto the 'lens'
**	screen_point are the coordinates of the point of the screen that the ray
**		points to
*/

union u_color	raytrace(const int x, const int y, const t_env *env)
{
	t_vec3			screen_point;
	union u_color	rgb;

	screen_point = (t_vec3) //ray direction / point on the 'screen' in world coords
	{
		(2.0 * (x + 0.5) / (float)env->disp.res.x - 1.0) * env->disp.aspect_ratio * sin(env->disp.fov.x / 2.0),
		(1.0 - 2.0 * (y + 0.5) / (float)env->disp.res.y) * sin(env->disp.fov.y / 2.0),
		1.0
	};
	(void)vec3_normalize(&screen_point);

	// should the intersection function also return the normal ?
	// should we pack another function pointer inside the object struct ?

	// normal = point at hit - center of sphere

	// intersec_point = cast_ray(env, &screen_point);
}

union u_color	render(t_env *env)
{
	int				i;
	int				j;
	unsigned long	px_id;
	union u_color	color;

	px_id = 0;
	i = -1;
	while (++i < env->disp.res.x)
	{
		j = -1;
		while (++j < env->disp.res.y)
		{
			color = raytrace(i, j, env);
			env->mlx->img.data[px_id] = color.intc;
			++px_id;
		}
	}
}