/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:48:12 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/29 18:09:54 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3		norm_sphere(const union u_object *obj, const t_vec3 *hit)
{
	t_vec3		tmp;

	tmp = *hit;
	flt3_sub(&tmp, &obj->sphere.orig);
	flt3_divf(&tmp, obj->sphere.radius);
	return (tmp);
}

t_vec3		norm_plane(const union u_object *obj, const t_vec3 *hit)
{
	(void)hit;
	return (obj->plane.norm);
}

t_vec3		norm_cylinder(const union u_object *obj, const t_vec3 *hit)
{
	t_vec3		diff;
	t_vec3		tmp;

	diff = obj->cylinder.end;
	flt3_sub(&diff, &obj->cylinder.org);
	tmp = *hit;
	flt3_sub(&tmp, &obj->cylinder.org);
	flt3_cross(&tmp, &diff);
	flt3_cross(&diff, &tmp);
	return (diff);
}

t_vec3		norm_cone(const union u_object *obj, const t_vec3 *hit)
{
	t_vec3		tmp;

	tmp = *hit;
	flt3_sub(&tmp, &obj->cone.org);
	flt3_cross(&tmp, &obj->cone.dir);
	flt3_multf(flt3_cross(&tmp, &obj->cone.dir), -1.);
	return (tmp);
}
