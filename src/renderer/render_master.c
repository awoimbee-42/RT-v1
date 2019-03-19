/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_master.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/03/02 02:16:43 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static uint32_t	*linerndr(t_env *env, int px_skip, uint32_t *restrict img, int v)
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

static uint32_t	*linecpy(int scrn_w, int px_skip, uint32_t *restrict img)
{
	uint32_t		tmp;

	if (scrn_w <= 20)
		__builtin_unreachable();
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

/*
**	Apparently linecpy overflows at the end...
**	One solution is to malloc more lines at the begining
**	Yes, it's bad.
**	But it works.
*/

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

#include <time.h>
int				render(t_env *env)
{
	uint32_t		i;
	clock_t			t;

	t = clock();
	env->px_skip = NB_PX_SKIP;
	while (env->px_skip > 0)
	{
		i = -1;
		while (++i != env->threads_nb)
			env->threads[i].ptr = SDL_CreateThread(&render_thread, "",
				&env->threads[i]);
		i = -1;
		while (++i != env->threads_nb)
			SDL_WaitThread(env->threads[i].ptr, NULL);
		SDL_UpdateTexture(env->sdl.texture, NULL, env->sdl.img,
			env->disp.w * sizeof(int));
		SDL_RenderCopy(env->sdl.renderer, env->sdl.texture, NULL, NULL);
		SDL_RenderPresent(env->sdl.renderer);
		env->px_skip -= PX_SKIP_STEP;
	}
	if (env->px_skip != -PX_SKIP_STEP)
	{
		t = clock() - t;
		ft_printf("%d rays in %fms\n", (env->disp.h * env->disp.w) * (1 + 1), (float)t/CLOCKS_PER_SEC * 1000); // the second 1 is depth
	}
	env->px_skip = NB_PX_SKIP;
	ft_bzero(env->sdl.img, env->disp.h * env->disp.w * sizeof(int32_t));
	return (0);
}
