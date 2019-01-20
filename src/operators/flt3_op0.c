/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:14:19 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/20 23:49:33 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

inline t_flt3		flt3_add(t_flt3 a, const t_flt3 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return (a);
}

inline t_flt3		flt3_sub(t_flt3 a, const t_flt3 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return (a);
}

inline t_flt3		flt3_mult(t_flt3 a, const t_flt3 b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return (a);
}

inline t_flt3		flt3_multf(t_flt3 a, const float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return (a);
}

inline t_flt3		flt3_div(t_flt3 a, const t_flt3 b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return (a);
}

inline t_flt3		flt3_divf(t_flt3 a, const float b)
{
	a.x /= b;
	a.y /= b;
	a.z /= b;
	return (a);
}
