/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:49:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 11:07:09 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float	flt3_dot(const t_flt3 *a, const t_flt3 *b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

float	flt3_mod(const t_flt3 *a)
{
	return (sqrtf(flt3_dot(a, a)));
}

void	flt3_normalize(t_flt3 *a)
{
	const float	length = flt3_mod(a);

	flt3_divf(a, length);
}

void	flt3_cross(t_flt3 *a, const t_flt3 *b)
{
	float	a1;
	float	a2;

	a1 = a->x;
	a2 = a->y;
	a->x = a->y * b->z - a->z * b->y;
	a->y = a->z * b->x - a1 * b->z;
	a->z = a1 * b->y - a2 * b->x;
}

void	flt3_divf(t_flt3 *a, const float b)
{
	a->x /= b;
	a->y /= b;
	a->z /= b;
}

void	flt3_addf(t_flt3 *a, const float b)
{
	a->x += b;
	a->y += b;
	a->z += b;
}
