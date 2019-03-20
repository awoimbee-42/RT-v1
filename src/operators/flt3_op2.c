/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 15:49:24 by awoimbee          #+#    #+#             */
/*   Updated: 2019/03/20 02:24:40 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_flt3		*flt3_divf(t_flt3 *restrict a, const float b)
{
	a->x /= b;
	a->y /= b;
	a->z /= b;
	return (a);
}

t_flt3		*flt3_addf(t_flt3 *restrict a, const float b)
{
	a->x += b;
	a->y += b;
	a->z += b;
	return (a);
}

t_flt3		*flt3_multf(t_flt3 *restrict a, const float b)
{
	a->x *= b;
	a->y *= b;
	a->z *= b;
	return (a);
}
