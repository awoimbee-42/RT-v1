/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 22:02:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/25 21:45:26 by awoimbee         ###   ########.fr       */
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

	oc = flt3_sub(ray.org, obj->sphere.orig);
	a = flt3_dot(ray.dir, ray.dir);
	b = 2.0 * flt3_dot(ray.dir, oc);
	c = flt3_dot(oc, oc) - (obj->sphere.radius * obj->sphere.radius);
	delta = b * b - 4 * a * c;
	if (delta < 0)
		return (-1);
	else if ((c = (-b - sqrt(delta)) / (2 * a)) > 0.1)
		return (c);
	return ((-b + sqrt(delta)) / (2 * a));
}

float		dist_plane(const union u_object *obj, const t_ray ray)
{
	float	denom;
	float	dist;

	denom = flt3_dot(obj->plane.norm, ray.dir);
	if (fabs(denom) > 1e-6)
	{
		dist = flt3_dot(
			flt3_sub(obj->plane.orig, ray.org),
			obj->plane.norm)
			/ denom;
		if (dist > 0)
			return (dist);
	}
	return (0);
}

float		dist_disk(const union u_object *obj, const t_ray ray)
{
	float	dist;
	t_vec3	v;
	float	disk2;

	if ((dist = dist_plane(obj, ray)) != -1)
	{
		v =
		flt3_sub(flt3_add(ray.org, flt3_multf(ray.dir, dist)), obj->disk.orig);
		disk2 = flt3_dot(v, v);
		if (disk2 <= obj->disk.radius2)
			return (dist);
	}
	return (0);
}

float		dist_cylinder(const union u_object *obj, const t_ray ray)
{
	t_flt3		x;
	t_flt3		y;
	float		d;
	t_flt3		diff;
	t_flt3		par;

	diff = flt3_sub(obj->cylinder.end, obj->cylinder.org);
	x = flt3_cross(flt3_sub(ray.org, obj->cylinder.org), diff);
	y = flt3_cross(ray.dir, diff);
	d = obj->cylinder.radius * obj->cylinder.radius * flt3_dot(diff, diff);
	par.x = flt3_dot(y, y);
	par.y = 2 * flt3_dot(x, y);
	par.z = flt3_dot(x, x) - d;
	d = par.y * par.y - 4 * par.x * par.z;
	if (d < 0)
		return (-1);
	diff.x = (-par.y - sqrt(d)) / (2 * par.x);
	diff.y = (-par.y + sqrt(d)) / (2 * par.x);
	if (diff.x > 0.1)
		return (diff.x);
	return (diff.y);
}
