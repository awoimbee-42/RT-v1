/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:48:12 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/26 19:03:22 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3		norm_sphere(const union u_object *obj, const t_vec3 hit)
{
	return (flt3_divf(flt3_sub(hit, obj->sphere.orig), obj->sphere.radius));
}

t_vec3		norm_plane(const union u_object *obj, const t_vec3 hit)
{
	(void)hit;
	return (obj->plane.norm);
}

t_vec3		norm_cylinder(const union u_object *obj, const t_vec3 hit)
{
	t_vec3		diff;

	diff = flt3_sub(obj->cylinder.end, obj->cylinder.org);
	return (flt3_cross(diff, flt3_cross(flt3_sub(hit, obj->cylinder.org)
					, diff)));
}

t_vec3		norm_cone(const union u_object *obj, const t_vec3 hit)
{
	return (flt3_cross(obj->cone.dir, flt3_cross(flt3_sub(hit
							, obj->cone.org), obj->cone.dir)));
}
