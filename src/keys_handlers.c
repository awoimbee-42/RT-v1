/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_handlers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/21 01:14:44 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		loop(void *ram)
{
	t_env	*env;

	env = (t_env*)ram;
	sleep(2);
	return (0);
}

int			key_pressed(int key, void *ram)
{
	t_env	*env;

	env = (t_env*)ram;
	fprintf(stdout, "key_pressed: %d\n", key);
	if (key == K_ESC)
	{
		free(env->light_arr);
		free(env->objs_arr);
		mlx_destroy_image(env->mlx->ptr, env->mlx->img.ptr);
		mlx_destroy_window(env->mlx->ptr, env->mlx->win);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

int			key_released(int key, void *ram)
{
	t_env	*env;

	env = (t_env*)ram;
	fprintf(stdout, "key_released: %d\n", key);
	if (key == K_UP)
		env->keys_pressed ^= 0x1;
	if (key == K_DWN)
		env->keys_pressed ^= 0x2;
	if (key == K_LFT)
		env->keys_pressed ^= 0x4;
	if (key == K_RGT)
		env->keys_pressed ^= 0x8;
	return (0);
}
