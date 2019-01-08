/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/08 14:05:07 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"


int		main(int argc, char **argv)
{
	t_mlx	mlx;
	t_env	env;
	int		i;

	init(&env, &mlx);
	read_argv(&env, argv, argc);
	i = -1;

	if (!(mlx.win = mlx_new_window(mlx.ptr,
	env.disp.res.x, env.disp.res.y, "RT-V1")));
		error(1);

	render(&mlx, &env);
	mlx_loop(mlx.ptr);
}

void	error(int	err_nb)
{
	if (err_nb == 0)
		perror("MiniLibX error: ");
	exit(EXIT_FAILURE);
}
