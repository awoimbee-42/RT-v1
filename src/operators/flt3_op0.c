/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:14:19 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 16:12:36 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_flt3		*flt3_add(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
	return (a);
}

t_flt3		*flt3_sub(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
	return (a);
}

t_flt3		*flt3_mult(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x *= b->x;
	a->y *= b->y;
	a->z *= b->z;
	return (a);
}

t_flt3		*flt3_div(t_flt3 *restrict a, const t_flt3 *restrict b)
{
	a->x /= b->x;
	a->y /= b->y;
	a->z /= b->z;
	return (a);
}
