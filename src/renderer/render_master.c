/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_master.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/07/14 15:43:36 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "rtv1.h"

static uint32_t	*linerndr(t_env *env, int px_skip, uint32_t *restrict img, int v)
{
	uint32_t		u;

	u = 0;
	while (u < env->disp.w && !env->stop)
	{
		if (!(*img & PX_RENDERED))
		{
			ft_mem32set(img, launch_ray(u, v, env, 1), px_skip);
			*img |= PX_RENDERED;
		}
		img += px_skip;
		u += px_skip;
	}
	img -= u - env->disp.w;

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
	thread->px_skip = NB_PX_SKIP;
	while (thread->px_skip > 0 && !thread->env->stop)
	{
		v = thread->line_start;
		px = thread->px_start;
		while (v < thread->line_end && !thread->env->stop)
		{
			px = linerndr(thread->env, thread->px_skip, px, v);
			px = linecpy(thread->env->disp.w, thread->px_skip, px);
			v += thread->px_skip;
		}
		thread->px_skip -= PX_SKIP_STEP;
	}
	return (0);
}

int				render(t_env *env)
{
	uint32_t		i;
	struct timespec	t[2];
	float			telapsed;

	clock_gettime(CLOCK_MONOTONIC, &t[0]);
	i = -1;
	while (++i != env->threads_nb)
		env->threads[i].ptr =
			SDL_CreateThread(&render_thread, "", &env->threads[i]);
	i = -1;
	while (++i != env->threads_nb)
		SDL_WaitThread(env->threads[i].ptr, NULL);
	if (!env->stop)
	{
		clock_gettime(CLOCK_MONOTONIC, &t[1]);
		telapsed = ((t[1].tv_sec - t[0].tv_sec) * 1e3)
			+ ((t[1].tv_nsec - t[0].tv_nsec) / 1e6);
		ft_printf("Time to render: %4.0fms\n", telapsed);
		supersample(env);
	}
	i = env->disp.w * env->disp.h;
	while (i-- != 0)
		env->sdl.img[i] &= 0x00FFFFFF;
	return (0);
}
