/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_op.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 18:26:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/25 17:59:29 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3			get_reflection(t_vec3 d, const t_vec3 n)
{
	return (flt3_sub(d, flt3_multf(n, 2. * flt3_dot(d, n))));
}

t_fcolor		light_drop(const t_fcolor light, const float dist)
{
	float		dist2;

	dist2 = dist * dist;
	return (
		(t_fcolor){
			light.x / dist2,
			light.y / dist2,
			light.z / dist2
		});
}

unsigned int	srgb(const t_fcolor color)
{
	return (
			((unsigned int)(color.z * 255) & 0xFFU)
			+ ((((unsigned int)(color.y * 255)) << 8) & 0xFF00)
			+ ((((unsigned int)(color.x * 255)) << 16) & 0xFF0000)
			+ (0U << 24));
}

t_fcolor	tone_map(const t_fcolor px)
{
	return (flt3_div(px, flt3_add(px, (t_fcolor){1, 1, 1})));
}

float	points_dist(const t_coords p1, const t_coords p2)
{
	return (sqrt(
		(p2.x - p1.x) * (p2.x - p1.x)
		+ (p2.y - p1.y) * (p2.y - p1.y)
		+ (p2.z - p1.z) * (p2.z - p1.z)));
}
