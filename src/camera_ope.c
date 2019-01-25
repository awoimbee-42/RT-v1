/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_ope.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:32:57 by cpoirier          #+#    #+#             */
/*   Updated: 2019/01/25 19:21:29 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		rotate_2d(t_vec3 *restrict v, const float angle, const int axis)
{
	float tmp;

	if (axis == 0)
	{
		tmp = v->z;
		v->z = cos(angle) * v->z - sin(angle) * v->y;
		v->y = cos(angle) * v->y + sin(angle) * tmp;
	}
	else if (axis == 1)
	{
		tmp = v->x;
		v->x = cos(angle) * v->x - sin(angle) * v->z;
		v->z = cos(angle) * v->z + sin(angle) * tmp;
	}
	else
	{
		tmp = v->x;
		v->x = cos(angle) * v->x - sin(angle) * v->y;
		v->y = cos(angle) * v->y + sin(angle) * tmp;
	}
}

void			apply_camera_rot(const t_env *env, t_vec3 *s)
{
	rotate_2d(s, env->camera.dir.x, 0);
	rotate_2d(s, env->camera.dir.y, 1);
	rotate_2d(s, env->camera.dir.z, 2);
}

void			move_camera(t_env *env, int dir)
{
	t_vec3	unit;

	unit = (t_vec3) {0., 0., 0.5};
	apply_camera_rot(env, &unit);
	if (dir == 0)
		env->camera.org = flt3_add(env->camera.org, unit);
	else if (dir == 1)
		env->camera.org = flt3_sub(env->camera.org, unit);
	else
	{
		rotate_2d(&unit, M_PI / 2, 1);
		unit.y = 0;
		if (dir == 2)
			env->camera.org = flt3_add(env->camera.org, unit);
		else
			env->camera.org = flt3_sub(env->camera.org, unit);
	}
}
