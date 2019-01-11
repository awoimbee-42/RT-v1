/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 18:26:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/11 21:47:01 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_fcolor		light_filter(const t_fcolor light, const t_fcolor surface)
{
	return (
		(t_fcolor)
		{
			light.r / surface.r,
			light.g / surface.g,
			light.b / surface.b
		}
	);
}

t_fcolor		light_mult(const t_fcolor color, const t_fcolor light)
{
	return (
		(t_fcolor)
		{
			color.r * light.r,
			color.g * light.g,
			color.b * light.b
		}
	);
}

t_fcolor		light_add(const t_fcolor c1, const t_fcolor c2)
{
	return (
		(t_fcolor)
		{
			c1.r + c2.r,
			c1.g + c2.g,
			c1.b + c2.b
		}
	);
}

t_fcolor		light_drop(const t_fcolor light, const float dist)
{
	float		dist2;

	dist2 = dist * dist;
	return (
		(t_fcolor)
		{
			light.r / dist2,
			light.g / dist2,
			light.b / dist2
		}
	);
}


int							srgb(t_fcolor color)
{
	return (
		(((int)(color.b * 255) & 0xFF)
		+ ((((int)(color.g * 255)) << 8) & 0xFF00)
		+ ((((int)(color.r * 255)) << 16) & 0xFF0000))
	);
}
