/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/21 18:47:33 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		loop(t_env *env)
{
	SDL_Event event;

	while (1)
	{
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
			exit_cleanly(env);
		if (event.type == SDL_KEYDOWN)
			key_pressed(event.key.keysym.sym, env);
	}
}

void		key_pressed(SDL_Keycode key, t_env *env)
{
	fprintf(stdout, "key_pressed: %d\n", key);
	if (key == SDLK_ESCAPE)
		exit_cleanly(env);
}

void		key_released(SDL_Keycode key, t_env *env)
{
	fprintf(stdout, "key_released: %d\n", key);
	if (key == K_UP)
		env->keys_pressed ^= 0x1;
	if (key == K_DWN)
		env->keys_pressed ^= 0x2;
	if (key == K_LFT)
		env->keys_pressed ^= 0x4;
	if (key == K_RGT)
		env->keys_pressed ^= 0x8;
}
