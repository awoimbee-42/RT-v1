/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dist.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 22:02:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/03/02 03:30:27 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float		dist_sphere(const union u_object *restrict obj, const t_ray *restrict ray)
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
	if (delta < 0.0001)
		return (NAN);
	if ((c = (-b - sqrtf(delta)) / (2 * a)) > 0.01)
		return (c);
	return ((-b + sqrtf(delta)) / (2 * a));
}

float		dist_plane(const union u_object *restrict obj, const t_ray *restrict ray)
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
	return (NAN);
}

float		dist_disk(const union u_object *obj, const t_ray *ray)
{
	float	dist;
	t_vec3	v;
	float	disk2;

	if ((dist = dist_plane(obj, ray)) != NAN)
	{
		v = ray->dir;
		flt3_multf(&v, dist);
		flt3_add(&v, &ray->org);
		flt3_sub(&v, &obj->disk.orig);
		disk2 = flt3_dot(&v, &v);
		if (disk2 <= obj->disk.radius2)
			return (dist);
	}
	return (NAN);
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
		return (NAN);
	diff.x = (-par.y - sqrtf(d)) / (2 * par.x);
	if (diff.x > 0.1)
		return (diff.x);
	diff.y = (-par.y + sqrtf(d)) / (2 * par.x);
	return (diff.y);
}

float		dist_cone(const union u_object *obj, const t_ray *ray)
{
	t_vec3		diff;
	t_vec3		par;
	float		cos2;
	float		dir_dot;
	float		org_dot;

	diff = ray->org;
	flt3_sub(&diff, &obj->cone.org);
	dir_dot = flt3_dot(&ray->dir, &obj->cone.dir);
	org_dot = flt3_dot(&diff, &obj->cone.dir);
	cos2 = cosf(obj->cone.angle) * cosf(obj->cone.angle);
	par.x = dir_dot * dir_dot - cos2;
	par.y = 2 * (dir_dot * org_dot - flt3_dot(&ray->dir, &diff) * cos2);
	par.z = org_dot * org_dot - flt3_dot(&diff, &diff) * cos2;
	dir_dot = par.y * par.y - 4 * par.x * par.z;
	if (dir_dot < 0)
		return (NAN);
	diff.y = (-par.y + sqrtf(dir_dot)) / (2 * par.x);
	if (diff.y > 0.001)
		return (diff.y);
	diff.x = (-par.y - sqrtf(dir_dot)) / (2 * par.x);
	return (diff.x);
}

float		dist_triangle(const union u_object *obj, const t_ray *ray) //straight outta wikipedia
{
	t_vec3	edge1, edge2, h, s, q;
	float a, f, u, v;

	edge1 = obj->triangle.vert1;
	flt3_sub(&edge1, &obj->triangle.vert0);
	edge2 = obj->triangle.vert2;
	flt3_sub(&edge2, &obj->triangle.vert0);
	h = ray->dir;
	flt3_cross(&h, &edge2);
	a = flt3_dot(&edge1, &h);
	if (-1e-5 < a && a < 1e-5) //ray-triangle are parallel
		return (NAN);
	f = 1.f / a;
	s = ray->org;
	flt3_sub(&s, &obj->triangle.vert0);
	u = f * flt3_dot(&s, &h);
	if (u < 0.f || u > 1.f)
		return (NAN);
	q = s;
	flt3_cross(&q, &edge1);
	v = f * flt3_dot(&ray->dir, &q);
	if (v < 0.0 || u + v > 1.0)
        return (NAN);
	f = f * flt3_dot(&edge2, &q);
	return (f);
}


//     q = s.crossProduct(edge1);
//     v = f * rayVector.dotProduct(q);
//     if (v < 0.0 || u + v > 1.0)
//         return false;
//     // At this stage we can compute t to find out where the intersection point is on the line.
//     float t = f * edge2.dotProduct(q);
//     if (t > EPSILON) // ray intersection
//     {
//         outIntersectionPoint = rayOrigin + rayVector * t;
//         return true;
//     }
//     else // This means that there is a line intersection but not a ray intersection.
//         return false;
// }
