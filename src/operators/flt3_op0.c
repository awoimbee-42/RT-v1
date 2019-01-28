/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:14:19 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 11:06:26 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		flt3_add(t_flt3 *a, const t_flt3 *b)
{
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
}

void		flt3_sub(t_flt3 *a, const t_flt3 *b)
{
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
}

void		flt3_mult(t_flt3 *a, const t_flt3 *b)
{
	a->x *= b->x;
	a->y *= b->y;
	a->z *= b->z;
}

void		flt3_multf(t_flt3 *a, const float b)
{
	a->x *= b;
	a->y *= b;
	a->z *= b;
}

void		flt3_div(t_flt3 *a, const t_flt3 *b)
{
	a->x /= b->x;
	a->y /= b->y;
	a->z /= b->z;
}
