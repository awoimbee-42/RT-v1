/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 16:47:43 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		check_keys(t_env *env)
{
	int_least8_t	redrw;

	redrw = 0;
	env->keys_pressed & BT_W && (redrw = 1) ? move_camera(env, 0) : 0;
	env->keys_pressed & BT_A && (redrw = 1) ? move_camera(env, 2) : 0;
	env->keys_pressed & BT_S && (redrw = 1) ? move_camera(env, 1) : 0;
	env->keys_pressed & BT_D && (redrw = 1) ? move_camera(env, 3) : 0;
	env->keys_pressed & BT_RIGHT && (redrw = 1) ? env->camera.dir.y -= 0.1 : 0;
	env->keys_pressed & BT_LEFT && (redrw = 1) ? env->camera.dir.y += 0.1 : 0;
	env->keys_pressed & BT_UP && (redrw = 1) ? env->camera.dir.x += 0.1 : 0;
	env->keys_pressed & BT_DOWN && (redrw = 1) ? env->camera.dir.x -= 0.1 : 0;
	env->keys_pressed & BT_Q && (redrw = 1) ? env->camera.org.y -= 0.25 : 0;
	env->keys_pressed & BT_E && (redrw = 1) ? env->camera.org.y += 0.25 : 0;
	if (redrw)
		render(env);
}

void		loop(t_env *env)
{
	SDL_Event	event;

	while (1)
	{
		usleep(500);
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				exit_cleanly(env);
			else if (event.type == SDL_KEYDOWN && !event.key.repeat)
				key_pressed(event.key.keysym.sym, env);
			else if (event.type == SDL_KEYUP)
				key_released(event.key.keysym.sym, env);
			continue;
		}
		check_keys(env);
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
