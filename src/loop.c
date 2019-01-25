/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/25 19:29:36 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

#include <stdio.h>

void		loop(t_env *env)
{
	SDL_Event event;

	while (1)
	{
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
			exit_cleanly(env);
		else if (event.type == SDL_KEYDOWN)// && !event.key.repeat)
			key_pressed(event.key.keysym.sym, env);
		else if (event.type == SDL_KEYUP)
			key_released(event.key.keysym.sym, env);
		else
			continue;
		//if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
			//printf("%d\n", event.type);

		if (env->keys_pressed & BT_W)
			move_camera(env, 0);
		if (env->keys_pressed & BT_A)
			move_camera(env, 2);
		if (env->keys_pressed & BT_S)
			move_camera(env, 1);
		if (env->keys_pressed & BT_D)
			move_camera(env, 3);
		if (env->keys_pressed & BT_RIGHT)
			env->camera.dir.y -= 0.1;
		if (env->keys_pressed & BT_LEFT)
			env->camera.dir.y += 0.1;
		if (env->keys_pressed & BT_UP)
			env->camera.dir.x += 0.1;
		if (env->keys_pressed & BT_DOWN)
			env->camera.dir.x -= 0.1;
		if (env->keys_pressed & BT_Q)
			env->camera.org.y -= 0.25;
		if (env->keys_pressed & BT_E)
			env->camera.org.y += 0.25;
		render(env);
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
	key == SDLK_q ? env->keys_pressed |= BT_Q : 0;
	key == SDLK_e ? env->keys_pressed |= BT_E : 0;
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
	key == SDLK_q ? env->keys_pressed ^= BT_Q : 0;
	key == SDLK_e ? env->keys_pressed ^= BT_E : 0;
}
