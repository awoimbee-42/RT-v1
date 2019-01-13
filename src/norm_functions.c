/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 22:48:12 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/12 17:54:06 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3		norm_sphere(const union u_object *obj, const t_vec3 hit)
{
	return (vec3_divf(vec3_sub(hit, obj->sphere.orig), obj->sphere.radius));
}

t_vec3		norm_plane(const union u_object *obj, const t_vec3 hit)
{
	(void)hit;
	return (obj->plane.norm);
}
