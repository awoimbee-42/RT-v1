/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_ope.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:32:57 by cpoirier          #+#    #+#             */
/*   Updated: 2019/02/27 19:02:04 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void		rotate_2d(t_vec3 *restrict v, const float angle, const int axis)
{
	float tmp;

	if (axis == 0)
	{
		tmp = v->z;
		v->z = cosf(angle) * v->z - sinf(angle) * v->y;
		v->y = cosf(angle) * v->y + sinf(angle) * tmp;
	}
	else if (axis == 1)
	{
		tmp = v->x;
		v->x = cosf(angle) * v->x - sinf(angle) * v->z;
		v->z = cosf(angle) * v->z + sinf(angle) * tmp;
	}
	else
	{
		tmp = v->x;
		v->x = cosf(angle) * v->x - sinf(angle) * v->y;
		v->y = cosf(angle) * v->y + sinf(angle) * tmp;
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
		flt3_add(&env->camera.org, &unit);
	else if (dir == 1)
		flt3_sub(&env->camera.org, &unit);
	else
	{
		rotate_2d(&unit, M_PI / 2, 1);
		unit.y = 0;
		if (dir == 2)
			flt3_add(&env->camera.org, &unit);
		else
			flt3_sub(&env->camera.org, &unit);
	}
}
