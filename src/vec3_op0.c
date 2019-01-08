/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_op0.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:14:19 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/08 14:37:29 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3		*vec3_add(t_vec3 *restrict a, const t_vec3 *restrict b)
{
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
	return (a);
}

t_vec3		*vec3_sub(t_vec3 *restrict a, const t_vec3 *restrict b)
{
	a->x -= b->x;
	a->y -= b->y;
	a->z -= b->z;
	return (a);
}

t_vec3		*vec3_multv(t_vec3 *restrict a, const t_vec3 *restrict b)
{
	a->x *= b->x;
	a->y *= b->y;
	a->z *= b->z;
	return (a);
}

t_vec3		*vec3_multf(t_vec3 *restrict a, const float b)
{
	a->x *= b;
	a->y *= b;
	a->z *= b;
	return (a);
}

t_vec3		*vec3_div(t_vec3 *restrict a, const t_vec3 *restrict b)
{
	a->x /= b->x;
	a->y /= b->y;
	a->z /= b->z;
	return (a);
}

float		vec3_dot(const t_vec3 *restrict a, const t_vec3 *restrict b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

float		vec3_mod(const t_vec3 *restrict a) //calculating the square root is sloooooow
{
	return sqrtf(vec3_dot(a, a));
}

t_vec3		*vec3_normalise(t_vec3 *restrict a)
{
	float length = vec3_mod(a);
	return (vec3_multf(a, 1.0 / length));
}
