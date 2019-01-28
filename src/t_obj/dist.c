/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 22:02:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 16:58:44 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float		dist_sphere(const union u_object *obj, const t_ray *ray)
{
	t_vec3		oc;
	float		a;
	float		b;
	float		c;
	float		delta;

	oc = ray->org;
	flt3_sub(&oc, &obj->sphere.orig);
	a = flt3_dot(&ray->dir, &ray->dir);
	b = 2.0 * flt3_dot(&ray->dir, &oc);
	c = flt3_dot(&oc, &oc) - (obj->sphere.radius * obj->sphere.radius);
	delta = b * b - 4 * a * c;
	if (delta < 0)
		return (-1);
	if ((c = (-b - sqrt(delta)) / (2 * a)) > 0.1)
		return (c);
	return ((-b + sqrt(delta)) / (2 * a));
}

float		dist_plane(const union u_object *obj, const t_ray *ray)
{
	float	denom;
	float	dist;
	t_vec3	tmp;

	denom = flt3_dot(&obj->plane.norm, &ray->dir);
	if (fabs(denom) > 1e-6)
	{
		tmp = obj->plane.orig;
		flt3_sub(&tmp, &ray->org);
		dist = flt3_dot(&tmp, &obj->plane.norm) / denom;
		if (dist > 0)
			return (dist);
	}
	return (0);
}

float		dist_disk(const union u_object *obj, const t_ray *ray)
{
	float	dist;
	t_vec3	v;
	float	disk2;

	if ((dist = dist_plane(obj, ray)) != -1)
	{
		v = ray->dir;
		flt3_multf(&v, dist);
		flt3_add(&v, &ray->org);
		flt3_sub(&v, &obj->disk.orig);
		disk2 = flt3_dot(&v, &v);
		if (disk2 <= obj->disk.radius2)
			return (dist);
	}
	return (0);
}

float		dist_cylinder(const union u_object *obj, const t_ray *ray)
{
	t_flt3		x;
	t_flt3		y;
	float		d;
	t_flt3		diff;
	t_flt3		par;

	diff = obj->cylinder.end;
	flt3_sub(&diff, &obj->cylinder.org);
	x = ray->org;
	flt3_sub(&x, &obj->cylinder.org);
	flt3_cross(&x, &diff);
	y = ray->dir;
	flt3_cross(&y, &diff);
	d = obj->cylinder.radius * obj->cylinder.radius * flt3_dot(&diff, &diff);
	par.x = flt3_dot(&y, &y);
	par.y = 2 * flt3_dot(&x, &y);
	par.z = flt3_dot(&x, &x) - d;
	d = par.y * par.y - 4 * par.x * par.z;
	if (d < 0)
		return (-1);
	diff.x = (-par.y - sqrt(d)) / (2 * par.x);
	diff.y = (-par.y + sqrt(d)) / (2 * par.x);
	if (diff.x > 0.1)
		return (diff.x);
	return (diff.y);
}

float		dist_cone(const union u_object *obj, const t_ray *ray)
{
	t_vec3		diff;
	t_vec3		par;
	double		cos2;
	double		dir_dot;
	double		org_dot;

	diff = ray->org;
	flt3_sub(&diff, &obj->cone.org);
	dir_dot = flt3_dot(&ray->dir, &obj->cone.dir);
	org_dot = flt3_dot(&diff, &obj->cone.dir);
	cos2 = cos(obj->cone.angle) * cos(obj->cone.angle);
	par.x = dir_dot * dir_dot - cos2;
	par.y = 2 * (dir_dot * org_dot - flt3_dot(&ray->dir, &diff) * cos2);
	par.z = org_dot * org_dot - flt3_dot(&diff, &diff) * cos2;
	dir_dot = par.y * par.y - 4 * par.x * par.z;
	if (dir_dot < 0)
		return (-1);
	diff.x = (-par.y - sqrt(dir_dot)) / (2 * par.x);
	diff.y = (-par.y + sqrt(dir_dot)) / (2 * par.x);
	if (diff.y > 0.1)
			return (diff.y);
	return (diff.x);
}
