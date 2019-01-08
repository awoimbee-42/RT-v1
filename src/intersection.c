/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 22:02:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/08 22:30:08 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float		hit_distance_sphere(const union u_object *obj, t_ray ray)
{
	float		a;
	float		b;
	float		c;
	float		discriminant;
	
	(void)vec3_sub(&ray.org, &obj->sphere.orig); //==oc
	a = vec3_dot(&ray.dir, &ray.dir);
	b = 2.0 * vec3_dot(&ray.org, &ray.dir);
	c = vec3_dot(&ray.org, &ray.org) - obj->sphere.radius * obj->sphere.radius;
	discriminant = b * b - 4 * a * c;
	if(discriminant < 0)
		return -1.0;
	else
		return (-b - sqrt(discriminant)) / (2.0 * a);
}
