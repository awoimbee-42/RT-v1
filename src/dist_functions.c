/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 22:02:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/12 13:32:56 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float		dist_sphere(const union u_object *obj, const t_ray ray)
{
	t_vec3		oc;
	float		a;
	float		b;
	float		c;
	float		delta;

	oc = vec3_sub(ray.org, obj->sphere.orig);
	a = vec3_dot(ray.dir, ray.dir);
	b = 2.0 * vec3_dot(ray.dir, oc);
	c = vec3_dot(oc, oc) - (obj->sphere.radius * obj->sphere.radius);
	delta = b * b - 4 * a * c;
	if (delta < 0)
		return (-1);
	else
		return ((-b - sqrt(delta)) / (2 * a));
}

float		dist_plane(const union u_object *obj, const t_ray ray)
{
	float	denom;
	float	dist;

	denom = vec3_dot(obj->plane.norm, ray.dir);
	if (fabsf(denom) > 1e-6)
	{
		dist = vec3_dot(vec3_sub(obj->plane.orig, ray.org), obj->plane.norm) / denom;
		if (dist > 0)
			return (dist);
	}
	return (-1);
}

float		dist_disk(const union u_object *obj, const t_ray ray)
{
	float	dist;
	t_vec3	v;
	float	disk2;

	if ((dist = dist_plane(obj, ray)) != -1)
	{
		v = vec3_sub(vec3_add(ray.org, vec3_multf(ray.dir, dist)), obj->disk.orig);
		disk2 = vec3_dot(v, v);
		if (disk2 <= obj->disk.radius2)
			return (dist);
	}
	return (-1);
}
