/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_op.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 18:26:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 11:17:52 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3			get_reflection(t_vec3 d, const t_vec3 n)
{
	t_vec3 pute;

	pute = n;
	flt3_multf(&pute, 2. * flt3_dot(&d, &n));

	flt3_sub(&d, &pute);
	return (d);
}

void		light_drop(t_fcolor *light, const float dist)
{
	float		dist2;

	dist2 = dist * dist;
	
			light->x /= dist2;
			light->y /= dist2;
			light->z /= dist2;
	
}

unsigned int	srgb(const t_fcolor color)
{
	return (
			((unsigned int)(color.z * 255) & 0xFFU)
			+ ((((unsigned int)(color.y * 255)) << 8) & 0xFF00)
			+ ((((unsigned int)(color.x * 255)) << 16) & 0xFF0000)
			+ (0U << 24));
}

void	tone_map(t_fcolor *px)
{
	t_vec3 pute;
	pute = *px;
	flt3_addf(&pute, 1);
	flt3_div(px, &pute);
}

float	points_dist(const t_coords p1, const t_coords p2)
{
	return (sqrt(
		(p2.x - p1.x) * (p2.x - p1.x)
		+ (p2.y - p1.y) * (p2.y - p1.y)
		+ (p2.z - p1.z) * (p2.z - p1.z)));
}
