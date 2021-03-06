/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_op.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 21:43:35 by arthur            #+#    #+#             */
/*   Updated: 2019/08/24 00:22:53 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_OP_H
# define LIGHT_OP_H
# include <datatypes.h>
# include <flt3_op.h>

static inline t_vec3			get_reflection(t_vec3 d, t_vec3 n)
{
	flt3_multf(&n, 2. * flt3_dot(&d, &n));
	flt3_sub(&d, &n);
	return (d);
}

static inline t_fcolor		*light_drop(t_fcolor *light, const float dist)
{
	float		dist2;

	dist2 = dist * dist;
	light->x /= dist2;
	light->y /= dist2;
	light->z /= dist2;
	return (light);
}

static inline uint32_t		srgb(const t_fcolor *color)
{
	return (
		((unsigned int)(color->z * 255) & 0xFFU)
		+ ((((unsigned int)(color->y * 255)) << 8) & 0xFF00)
		+ ((((unsigned int)(color->x * 255)) << 16) & 0xFF0000)
		+ (0U << 24));
}

/*
**	The power of 1/2.2 thing is gamma correction
**	Possible optimization: do sqrt(x) instead of pow(x, 1/2.2) (less accurate)
*/

static inline void			tone_map(t_fcolor *px)
{
	px->x = pow(px->x / (px->x + 1), 1. / 2.2);
	px->y = pow(px->y / (px->y + 1), 1. / 2.2);
	px->z = pow(px->z / (px->z + 1), 1. / 2.2);
}

#endif
