/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 00:40:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/07/11 16:07:27 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		check_keys(t_env *env)
{
	if (env->keys)
		env->stop = 1;
	env->keys & BT_W ? move_camera(env, 0) : 0;
	env->keys & BT_A ? move_camera(env, 2) : 0;
	env->keys & BT_S ? move_camera(env, 1) : 0;
	env->keys & BT_D ? move_camera(env, 3) : 0;
	env->keys & BT_RIGHT ? env->camera.dir.y -= 0.1 : 0;
	env->keys & BT_LEFT ? env->camera.dir.y += 0.1 : 0;
	env->keys & BT_UP ? env->camera.dir.x += 0.1 : 0;
	env->keys & BT_DOWN ? env->camera.dir.x -= 0.1 : 0;
	env->keys & BT_Q ? env->camera.org.y -= 0.25 : 0;
	env->keys & BT_E ? env->camera.org.y += 0.25 : 0;
	if (env->stop)
	{
		SDL_WaitThread(env->rndr, NULL);
		env->stop = 0;
		env->rndr = SDL_CreateThread((int (*)(void *))&render, "", env);
	}
}

void		check_controller(t_env *env, SDL_GameController *cntrlr)
{
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTY) < -20000
	&& (env->stop = 1) ? move_camera(env, 0) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTY) > 20000
	&& (env->stop = 1) ? move_camera(env, 1) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTX) < -20000
	&& (env->stop = 1) ? move_camera(env, 2) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_LEFTX) > 20000
	&& (env->stop = 1) ? move_camera(env, 3) : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTY) < -20000
	&& (env->stop = 1) ? env->camera.dir.x += 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTY) > 20000
	&& (env->stop = 1) ? env->camera.dir.x -= 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTX) < -20000
	&& (env->stop = 1) ? env->camera.dir.y += 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_RIGHTX) > 20000
	&& (env->stop = 1) ? env->camera.dir.y -= 0.1 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 20000
	&& (env->stop = 1) ? env->camera.org.y -= 0.25 : 0;
	SDL_GameControllerGetAxis(cntrlr, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 20000
	&& (env->stop = 1) ? env->camera.org.y += 0.25 : 0;
	if (env->stop)
	{
		SDL_WaitThread(env->rndr, NULL);
		env->stop = 0;
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
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.data1 > 20 && event.window.data2 > 20)
				resize(env);
			continue;
		}
		check_keys(env);
		check_controller(env, controller);

		SDL_UpdateTexture(env->sdl.texture, NULL, env->sdl.img, env->disp.w * sizeof(int));
		SDL_RenderCopy(env->sdl.renderer, env->sdl.texture, NULL, NULL);
		SDL_RenderPresent(env->sdl.renderer);
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
