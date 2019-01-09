/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_op1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:49:07 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/09 22:49:28 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

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
