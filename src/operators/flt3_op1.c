/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flt3_op1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:49:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/20 23:35:59 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

inline float	flt3_dot(const t_flt3 a, const t_flt3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline float	flt3_mod(const t_flt3 a)
{
	return sqrtf(flt3_dot(a, a));
}

inline t_flt3	flt3_normalize(t_flt3 a)
{
	float length = flt3_mod(a);
	return (flt3_divf(a, length));
}
