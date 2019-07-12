/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_master.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 12:15:44 by awoimbee          #+#    #+#             */
/*   Updated: 2019/07/11 16:18:32 by awoimbee         ###   ########.fr       */
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
			ft_mem32set(img, launch_ray(u, v, env), px_skip);
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
		px = linerndr(thread->env, thread->px_skip, px, v);
		v += thread->px_skip;
		while (v < thread->line_end && !thread->env->stop)
		{
			px = linecpy(thread->env->disp.w, thread->px_skip, px);
			px = linerndr(thread->env, thread->px_skip, px, v);
			v += thread->px_skip;

		}
		thread->px_skip -= PX_SKIP_STEP;
	}
	return (0);
}

/* Right now supersampling is single threaded */
void			supersample(t_env *env)
{
	uint32_t	c[2];
	uint32_t	*px;
	uint32_t	*end;
	uint32_t	rgb_sum[3];

	ft_bzero(c, 2* sizeof(uint32_t));
	px = env->sdl.img;
	end = &px[env->disp.w * env->disp.h];
	env->supersampling_rate = 2;
	while (!env->stop && env->supersampling_rate != 20)
	{
		ft_bzero(rgb_sum, 3 * sizeof(uint32_t));
		for (int i = 0; i < env->supersampling_rate; ++i)
		{
			for (int j = 0; j < env->supersampling_rate; ++j) {
				*px = launch_ray(c[0], c[1] + j, env);
				rgb_sum[0] += ((uint8_t*)px)[0];
				rgb_sum[1] += ((uint8_t*)px)[1];
				rgb_sum[2] += ((uint8_t*)px)[2];
			}
			++c[0];
		}
		rgb_sum[0] /= env->supersampling_rate * env->supersampling_rate;
		rgb_sum[1] /= env->supersampling_rate * env->supersampling_rate;
		rgb_sum[2] /= env->supersampling_rate * env->supersampling_rate;
		*px = rgb_sum[0] + (rgb_sum[1] << 8) + (rgb_sum[2] << 16);
		++px;

		if (c[0] == env->disp.w * env->supersampling_rate)
		{
			c[0] = 0;
			c[1] += env->supersampling_rate;
		}

		if (px == end)
		{
			ft_printf("Supersampled to %dx\n", env->supersampling_rate);
			px = env->sdl.img;
			++env->supersampling_rate;
			ft_bzero(c, 2 * sizeof(uint32_t));
		}
	}
	env->supersampling_rate = 1;
}

int				render(t_env *env)
{
	uint32_t		i;
	clock_t			t;

	t = clock();
	i = -1;
	while (++i != env->threads_nb)
		env->threads[i].ptr = SDL_CreateThread(&render_thread, "",
				&env->threads[i]);
	i = -1;
	while (++i != env->threads_nb)
		SDL_WaitThread(env->threads[i].ptr, NULL);
	if (!env->stop)
	{
		t = clock() - t;
		ft_printf("Time to render: %fms\n",
			(float)t/CLOCKS_PER_SEC * 1000);
		supersample(env);
	}
	i = env->disp.w * env->disp.h;
	while (i-- != 0)
		env->sdl.img[i] &= 0x00FFFFFF;
	return (0);
}
