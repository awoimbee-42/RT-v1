/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 18:26:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/02/28 19:41:03 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3			get_reflection(t_vec3 d, t_vec3 n)
{
	flt3_multf(&n, 2. * flt3_dot(&d, &n));
	flt3_sub(&d, &n);
	return (d);
}

t_fcolor		*light_drop(t_fcolor *light, const float dist)
{
	float		dist2;

	dist2 = dist * dist;
	light->x /= dist2;
	light->y /= dist2;
	light->z /= dist2;
	return (light);
}

uint32_t		srgb(const t_fcolor *color)
{
	return (
		((unsigned int)(color->z * 255) & 0xFFU)
		+ ((((unsigned int)(color->y * 255)) << 8) & 0xFF00)
		+ ((((unsigned int)(color->x * 255)) << 16) & 0xFF0000)
		+ (0U << 24));
}

void			tone_map(t_fcolor *px)
{
	px->x = px->x / (px->x + 1);
	px->y = px->y / (px->y + 1);
	px->z = px->z / (px->z + 1);
}
