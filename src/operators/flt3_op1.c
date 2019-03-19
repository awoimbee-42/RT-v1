/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:49:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/02/07 17:34:41 by awoimbee         ###   ########.fr       */
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

float	flt3_mod2(const t_flt3 *a)
{
	return (a->x * a->x + a->y * a->y + a->z * a->z);
}

/*
**	WILL return a NaN vector when you pass a null vector
*/

t_flt3	*flt3_normalize(t_flt3 *a)
{
	flt3_divf(a, sqrt(a->x * a->x + a->y * a->y + a->z * a->z));
	return (a);
}

t_flt3	*flt3_cross(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	float	a1;
	float	a2;

	a1 = a->x;
	a2 = a->y;
	a->x = a->y * b->z - a->z * b->y;
	a->y = a->z * b->x - a1 * b->z;
	a->z = a1 * b->y - a2 * b->x;
	return (a);
}
