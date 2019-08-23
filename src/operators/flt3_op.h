/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/23 21:41:50 by arthur            #+#    #+#             */
/*   Updated: 2019/08/24 00:22:55 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLT3_OP_H
# define FLT3_OP_H
# include <datatypes.h>

static inline t_flt3		*flt3_divf(t_flt3 *restrict a, const float b)
{
	a->x /= b;
	a->y /= b;
	a->z /= b;
	return (a);
}

static inline t_flt3		*flt3_addf(t_flt3 *restrict a, const float b)
{
	a->x += b;
	a->y += b;
	a->z += b;
	return (a);
}

static inline t_flt3		*flt3_multf(t_flt3 *restrict a, const float b)
{
	a->x *= b;
	a->y *= b;
	a->z *= b;
	return (a);
}

static inline t_flt3		*flt3_add(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
	return (a);
}

static inline t_flt3		*flt3_sub(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
	return (a);
}

static inline t_flt3		*flt3_mult(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x *= b->x;
	a->y *= b->y;
	a->z *= b->z;
	return (a);
}

static inline t_flt3		*flt3_div(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x /= b->x;
	a->y /= b->y;
	a->z /= b->z;
	return (a);
}

static inline float	flt3_dot(const t_flt3 *a, const t_flt3 *b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

static inline float	flt3_mod(const t_flt3 *a)
{
	return (sqrtf(flt3_dot(a, a)));
}

static inline float	flt3_mod2(const t_flt3 *a)
{
	return (a->x * a->x + a->y * a->y + a->z * a->z);
}

/*
**	WILL return a NaN vector when you pass a null vector
*/

static inline t_flt3	*flt3_normalize(t_flt3 *a)
{
	float	div;

	div = sqrtf(a->x * a->x + a->y * a->y + a->z * a->z);
	a->x /= div;
	a->y /= div;
	a->z /= div;
	return (a);
}

static inline t_flt3	*flt3_cross(t_flt3 *restrict a, const t_flt3 *restrict b)
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

#endif
