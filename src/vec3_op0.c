/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_op0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:14:19 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 18:28:47 by awoimbee         ###   ########.fr       */
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

inline float		vec3_dot(const t_vec3 a, const t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline float		vec3_mod(const t_vec3 a) //calculating the square root is sloooooow
{
	return sqrtf(vec3_dot(a, a));
}

inline t_vec3		vec3_normalize(t_vec3 a)
{
	float length = vec3_mod(a);
	return (vec3_multf(a, 1.0 / length));
}
