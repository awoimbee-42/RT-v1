/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/21 20:45:13 by awoimbee         ###   ########.fr       */
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
		if (event.type == SDL_KEYDOWN && !event.key.repeat)
			key_pressed(event.key.keysym.sym, env);
		if (event.type == SDL_KEYUP)
			key_released(event.key.keysym.sym, env);
	}
}

void		key_pressed(SDL_Keycode key, t_env *env)
{
	ft_printf("key_pressed: %b\n", key);
	if (key == SDLK_ESCAPE)
		exit_cleanly(env);
	key == SDLK_w ? env->keys_pressed |= BT_W : 0;
	key == SDLK_a ? env->keys_pressed |= BT_A : 0;
	key == SDLK_s ? env->keys_pressed |= BT_S : 0;
	key == SDLK_d ? env->keys_pressed |= BT_D : 0;
}

void		key_released(SDL_Keycode key, t_env *env)
{
	key == SDLK_w ? env->keys_pressed ^= BT_W : 0;
	key == SDLK_a ? env->keys_pressed ^= BT_A : 0;
	key == SDLK_s ? env->keys_pressed ^= BT_S : 0;
	key == SDLK_d ? env->keys_pressed ^= BT_D : 0;
}
