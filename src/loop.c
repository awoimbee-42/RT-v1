/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/25 23:07:50 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

#include <stdio.h>

void		loop(t_env *env)
{
	SDL_Event	event;
	int			rerender;

	while (1)
	{
		rerender = 0;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				exit_cleanly(env);
			else if (event.type == SDL_KEYDOWN)// && !event.key.repeat)
				key_pressed(event.key.keysym.sym, env);
			else if (event.type == SDL_KEYUP)
				key_released(event.key.keysym.sym, env);
			continue;
		}
		env->keys_pressed & BT_W && (rerender = 1)? move_camera(env, 0) : 0;
		env->keys_pressed & BT_A && (rerender = 1)? move_camera(env, 2) : 0;
		env->keys_pressed & BT_S && (rerender = 1)? move_camera(env, 1) : 0;
		env->keys_pressed & BT_D && (rerender = 1)? move_camera(env, 3) : 0;

		env->keys_pressed & BT_RIGHT && (rerender = 1) ? env->camera.dir.y -= 0.1 : 0;
		env->keys_pressed & BT_LEFT && (rerender = 1) ? env->camera.dir.y += 0.1 : 0;
		env->keys_pressed & BT_UP && (rerender = 1) ? env->camera.dir.x += 0.1 : 0;
		env->keys_pressed & BT_DOWN && (rerender = 1) ? env->camera.dir.x -= 0.1 : 0;
		env->keys_pressed & BT_Q && (rerender = 1) ? env->camera.org.y -= 0.25 : 0;
		env->keys_pressed & BT_E && (rerender = 1) ? env->camera.org.y += 0.25 : 0;
		if (rerender)
			render(env);
	}
}

void		key_pressed(SDL_Keycode key, t_env *env)
{
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
