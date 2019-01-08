/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/08 15:44:44 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

/*
**	raytrace() shoots the rays from the 'camera' onto the 'lens'
**	screen_point are the coordinates of the point of the screen that the ray
**		points to
*/

union u_color	raytrace(const int x, const int y, const t_env *env)
{
	t_vec3		screen_point;


	screen_point = (t_vec3) //ray direction / point on the 'screen' in world coords
	{
		(2.0 * (x + 0.5) / (float)env->disp.res.x - 1.0) * env->disp.aspect_ratio * sin(env->disp.fov.x / 2.0),
		(1.0 - 2.0 * (y + 0.5) / (float)env->disp.res.y) * sin(env->disp.fov.y / 2.0),
		1.0
	};
	(void)vec3_normalise(&screen_point);

	if (screen_point.y < 0)
	{

	}
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
			env->mlx->img.data[px_id] = color.col;
			++px_id;
		}
	}
}