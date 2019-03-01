/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/03/01 15:36:44 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		check_keys(t_env *env)
{
	env->keys & BT_W && !(env->px_skip = 0) ? move_camera(env, 0) : 0;
	env->keys & BT_A && !(env->px_skip = 0) ? move_camera(env, 2) : 0;
	env->keys & BT_S && !(env->px_skip = 0) ? move_camera(env, 1) : 0;
	env->keys & BT_D && !(env->px_skip = 0) ? move_camera(env, 3) : 0;
	env->keys & BT_RIGHT && !(env->px_skip = 0) ? env->camera.dir.y -= 0.1 : 0;
	env->keys & BT_LEFT && !(env->px_skip = 0) ? env->camera.dir.y += 0.1 : 0;
	env->keys & BT_UP && !(env->px_skip = 0) ? env->camera.dir.x += 0.1 : 0;
	env->keys & BT_DOWN && !(env->px_skip = 0) ? env->camera.dir.x -= 0.1 : 0;
	env->keys & BT_Q && !(env->px_skip = 0) ? env->camera.org.y -= 0.25 : 0;
	env->keys & BT_E && !(env->px_skip = 0) ? env->camera.org.y += 0.25 : 0;
	if (!env->px_skip)
	{
		SDL_WaitThread(env->rndr, NULL);
		env->rndr = SDL_CreateThread((int (*)(void *))&render, "", env);
	}
}

void		check_controller(t_env *env, SDL_GameController *cntrlr)
{
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTY) < -20000
	&& !(env->px_skip = 0) ? move_camera(env, 0) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTY) > 20000
	&& !(env->px_skip = 0) ? move_camera(env, 1) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTX) < -20000
	&& !(env->px_skip = 0) ? move_camera(env, 2) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTX) > 20000
	&& !(env->px_skip = 0) ? move_camera(env, 3) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTY) < -20000
	&& !(env->px_skip = 0) ? env->camera.dir.x += 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTY) > 20000
	&& !(env->px_skip = 0) ? env->camera.dir.x -= 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTX) < -20000
	&& !(env->px_skip = 0) ? env->camera.dir.y += 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTX) > 20000
	&& !(env->px_skip = 0) ? env->camera.dir.y -= 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 20000
	&& !(env->px_skip = 0) ? env->camera.org.y -= 0.25 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 20000
	&& !(env->px_skip = 0) ? env->camera.org.y += 0.25 : 0;
	if (!env->px_skip)
	{
		SDL_WaitThread(env->rndr, NULL);
		env->rndr = SDL_CreateThread((int (*)(void *))&render, "", env);
	}
}

/*
**	if we where able to, we would put usleep(500); in the while
**		to stop hogging the cpu...
*/

void		loop(t_env *env, SDL_GameController *controller)
{
	SDL_Event	event;

	env->rndr = SDL_CreateThread((int (*)(void *))&render, "", env);
	while (1)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				exit_cleanly(env);
			else if (event.type == SDL_KEYDOWN && !event.key.repeat)
				key_pressed(event.key.keysym.sym, env);
			else if (event.type == SDL_KEYUP)
				key_released(event.key.keysym.sym, env);
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
				resize(env);
			continue;
		}
		check_keys(env);
		check_controller(env, controller);
		usleep(33000);
	}
}

void		key_pressed(SDL_Keycode key, t_env *env)
{
	if (key == SDLK_ESCAPE)
		exit_cleanly(env);
	key == SDLK_w ? env->keys |= BT_W : 0;
	key == SDLK_a ? env->keys |= BT_A : 0;
	key == SDLK_s ? env->keys |= BT_S : 0;
	key == SDLK_d ? env->keys |= BT_D : 0;
	key == SDLK_UP ? env->keys |= BT_UP : 0;
	key == SDLK_DOWN ? env->keys |= BT_DOWN : 0;
	key == SDLK_LEFT ? env->keys |= BT_LEFT : 0;
	key == SDLK_RIGHT ? env->keys |= BT_RIGHT : 0;
	key == SDLK_q ? env->keys |= BT_Q : 0;
	key == SDLK_e ? env->keys |= BT_E : 0;
}

void		key_released(SDL_Keycode key, t_env *env)
{
	key == SDLK_w ? env->keys ^= BT_W : 0;
	key == SDLK_a ? env->keys ^= BT_A : 0;
	key == SDLK_s ? env->keys ^= BT_S : 0;
	key == SDLK_d ? env->keys ^= BT_D : 0;
	key == SDLK_LEFT ? env->keys ^= BT_LEFT : 0;
	key == SDLK_DOWN ? env->keys ^= BT_DOWN : 0;
	key == SDLK_UP ? env->keys ^= BT_UP : 0;
	key == SDLK_RIGHT ? env->keys ^= BT_RIGHT : 0;
	key == SDLK_q ? env->keys ^= BT_Q : 0;
	key == SDLK_e ? env->keys ^= BT_E : 0;
}
