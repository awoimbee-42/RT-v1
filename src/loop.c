/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/22 12:26:30 by cpoirier         ###   ########.fr       */
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
	
		if (env->keys_pressed & BT_W)
		{
			env->camera.org.y += 0.5;
			render(env);
		}
		if (env->keys_pressed & BT_A)
		{
			env->camera.org.x -= 0.5;
			render(env);
		}
		if (env->keys_pressed & BT_S)
		{
			env->camera.org.y -= 0.5;
			render(env);
		}
		if (env->keys_pressed & BT_D)
		{
			env->camera.org.x += 0.5;
			render(env);
		}
		if (env->keys_pressed & BT_RIGHT)
		{
			env->camera.dir.z -= 0.1;
			render(env);
		}
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
	key == SDLK_UP ? env->keys_pressed |= BT_UP : 0;
	key == SDLK_DOWN ? env->keys_pressed |= BT_DOWN : 0;
	key == SDLK_LEFT ? env->keys_pressed |= BT_LEFT : 0;
	key == SDLK_RIGHT ? env->keys_pressed |= BT_RIGHT : 0;
}

void		key_released(SDL_Keycode key, t_env *env)
{
	key == SDLK_w ? env->keys_pressed ^= BT_W : 0;
	key == SDLK_a ? env->keys_pressed ^= BT_A : 0;
	key == SDLK_s ? env->keys_pressed ^= BT_S : 0;
	key == SDLK_d ? env->keys_pressed ^= BT_D : 0;
	key == SDLK_LEFT ? env->keys_pressed ^= BT_LEFT : 0;
	key == SDLK_DOWN ? env->keys_pressed ^= BT_DOWN : 0;
	key == SDLK_UP ? env->keys_pressed ^= BT_UP : 0;
	key == SDLK_RIGHT ? env->keys_pressed ^= BT_RIGHT : 0;
}
