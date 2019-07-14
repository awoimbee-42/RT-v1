/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supersample.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/14 15:08:08 by awoimbee          #+#    #+#             */
/*   Updated: 2019/07/14 15:46:32 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include <rtv1.h>

static int		supersample_thread(void *vthread)
{
	t_thread		*thread;
	uint32_t		*px;
	uint32_t		*end;
	uint32_t		rgb_sum[3];
	uint32_t		super_px[2];

	thread = (t_thread*)vthread;
	thread->px_skip = 2;
	px = &thread->env->sdl.img[thread->line_start * thread->env->disp.w];
	end = &thread->env->sdl.img[thread->line_end * thread->env->disp.w];
	super_px[0] = 0;
	super_px[1] = thread->line_start * thread->px_skip;
	while (!thread->env->stop && thread->px_skip != 10)
	{
		ft_bzero(rgb_sum, 3 * sizeof(uint32_t));
		for (int i = 0; i < thread->px_skip; ++i)
		{
			for (int j = 0; j < thread->px_skip; ++j) {
				*px = launch_ray(super_px[0], super_px[1] + j, thread->env, thread->px_skip);
				rgb_sum[0] += ((uint8_t*)px)[0];
				rgb_sum[1] += ((uint8_t*)px)[1];
				rgb_sum[2] += ((uint8_t*)px)[2];
			}
			++super_px[0];
		}
		rgb_sum[0] /= thread->px_skip * thread->px_skip;
		rgb_sum[1] /= thread->px_skip * thread->px_skip;
		rgb_sum[2] /= thread->px_skip * thread->px_skip;
		*px = rgb_sum[0] + (rgb_sum[1] << 8) + (rgb_sum[2] << 16);
		++px;

		if (super_px[0] == thread->env->disp.w * thread->px_skip)
		{
			super_px[0] = 0;
			super_px[1] += thread->px_skip;
		}
		if (px == end)
		{
			px = &thread->env->sdl.img[thread->line_start * thread->env->disp.w];
			++thread->px_skip;
			super_px[0] = 0;
			super_px[1] = thread->line_start * thread->px_skip;
		}
	}
	return (0);
}

void				supersample(t_env *env)
{
	uint32_t		i;
	int				max_sampling;
	struct timespec	t[2];
	float			telapsed;

	clock_gettime(CLOCK_MONOTONIC, &t[0]);
	i = -1;
	while (++i != env->threads_nb)
		env->threads[i].ptr =
			SDL_CreateThread(&supersample_thread, "", &env->threads[i]);
	i = -1;
	max_sampling = 1;
	while (++i != env->threads_nb)
	{
		SDL_WaitThread(env->threads[i].ptr, NULL);
		if (env->threads[i].px_skip > max_sampling)
			max_sampling = env->threads[i].px_skip;
	}
	if (--max_sampling > 1)
	{
		clock_gettime(CLOCK_MONOTONIC, &t[1]);
		telapsed = ((t[1].tv_sec - t[0].tv_sec) * 1e3)
			+ ((t[1].tv_nsec - t[0].tv_nsec) / 1e6);
		ft_printf("\tSupersampled to %2dx in %4.0fms\n", max_sampling, telapsed);
	}
}