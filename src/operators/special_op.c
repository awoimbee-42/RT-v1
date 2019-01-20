/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_op.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 18:26:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/20 23:47:21 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_fcolor		light_drop(const t_fcolor light, const float dist)
{
	float		dist2;

	dist2 = dist * dist;
	return (
		(t_fcolor)
		{
			light.x / dist2,
			light.y / dist2,
			light.z / dist2
		}
	);
}

inline int		srgb(const t_fcolor color)
{
	return (
		(((int)(color.z * 255) & 0xFF)
		+ ((((int)(color.y * 255)) << 8) & 0xFF00)
		+ ((((int)(color.x * 255)) << 16) & 0xFF0000))
	);
}

inline t_fcolor	tone_map(const t_fcolor px)
{
	return (flt3_div(px, flt3_add(px, (t_fcolor){1, 1, 1})));
}

inline float	points_dist(const t_coords p1, const t_coords p2)
{
	return (sqrt(
		(p2.x - p1.x) * (p2.x - p1.x)
		+ (p2.y - p1.y) * (p2.y - p1.y)
		+ (p2.z - p1.z) * (p2.z - p1.z))
	);
}
