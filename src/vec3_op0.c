/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_op0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:14:19 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 22:48:53 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

inline t_vec3		vec3_add(t_vec3 a, const t_vec3 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return (a);
}

inline t_vec3		vec3_sub(t_vec3 a, const t_vec3 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return (a);
}

inline t_vec3		vec3_multv(t_vec3 a, const t_vec3 b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	return (a);
}

inline t_vec3		vec3_multf(t_vec3 a, const float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return (a);
}

inline t_vec3		vec3_div(t_vec3 a, const t_vec3 b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	return (a);
}
