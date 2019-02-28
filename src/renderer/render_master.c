/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_master.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/02/28 20:54:28 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static uint32_t	*linerndr(t_env *env, int px_skip, uint32_t *img, int v)
{
	uint32_t		u;

	if (px_skip)
	{
		u = 0;
		while (u < env->disp.w)
		{
			if (!(*img & PX_RENDERED))
			{
				ft_mem32set(img, launch_ray(u, v, env), px_skip);
				*img |= PX_RENDERED;
			}
			img += px_skip;
			u += px_skip;
		}
		img -= u - env->disp.w;
	}
	return (img);
}

uint32_t		*linecpy(int scrn_w, int px_skip, uint32_t *img)
{
	uint32_t		tmp;

	while (--px_skip > 0)
	{
		tmp = scrn_w;
		while (tmp--)
		{
			*img = img[-scrn_w] & ~PX_RENDERED;
			++img;
		}
	}
	return (img);
}

static int		render_thread(void *vthread)
{
	t_thread		*thread;
	uint32_t		*px;
	uint32_t		v;

	thread = (t_thread*)vthread;
	v = thread->line_start;
	px = thread->px_start;
	while (v < thread->line_end && thread->env->px_skip)
	{
		px = linerndr(thread->env, thread->env->px_skip, px, v);
		px = linecpy(thread->env->disp.w, thread->env->px_skip, px);
		v += thread->env->px_skip;
	}
	return (0);
}

int				render(t_env *env)
{
	int			i;

	env->px_skip = NB_PX_SKIP;
	while (env->px_skip > 0)
	{
		i = -1;
		while (++i != THREAD_NB)
			env->threads[i].ptr = SDL_CreateThread(&render_thread, "",
				&env->threads[i]);
		i = -1;
		while (++i != THREAD_NB)
			SDL_WaitThread(env->threads[i].ptr, NULL);
		SDL_UpdateTexture(env->sdl.texture, NULL, env->sdl.img,
			env->disp.w * sizeof(int));
		SDL_RenderCopy(env->sdl.renderer, env->sdl.texture, NULL, NULL);
		SDL_RenderPresent(env->sdl.renderer);
		env->px_skip -= 2;
	}
	env->px_skip = NB_PX_SKIP;
	ft_bzero(env->sdl.img, env->disp.h * env->disp.w * sizeof(int32_t));
	return (0);
}
