/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:48:12 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 16:56:14 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3		norm_sphere(const union u_object *obj, const t_vec3 *hit)
{
	t_vec3		pute;

	pute = *hit;
	flt3_sub(&pute, &obj->sphere.orig);
	flt3_divf(&pute, obj->sphere.radius);
	return (pute);
}

t_vec3		norm_plane(const union u_object *obj, const t_vec3 *hit)
{
	(void)hit;
	return (obj->plane.norm);
}

t_vec3		norm_cylinder(const union u_object *obj, const t_vec3 *hit)
{
	t_vec3		diff;
	t_vec3		pute;

	diff = obj->cylinder.end;
	flt3_sub(&diff, &obj->cylinder.org);
	
	pute = *hit;
	flt3_sub(&pute, &obj->cylinder.org);
	flt3_cross(&pute, &diff);
	flt3_cross(&diff, &pute);
	return (diff);
}

t_vec3		norm_cone(const union u_object *obj, const t_vec3 *hit)
{
	t_vec3 pute;

	pute = 		*hit;
	flt3_sub(&pute, &obj->cone.org);
	flt3_cross(&pute, &obj->cone.dir);
	flt3_cross(&pute, &obj->cone.dir);
	return (pute);
}
