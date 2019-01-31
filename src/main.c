/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 10:36:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/31 02:05:15 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		main(int argc, char **argv)
{
	t_env	*env;

	if (argc != 2)
		usage();
	if (!(env = malloc(sizeof(t_env))))
		error(MALLOC_ERR);
	parse_scene(env, argv[1]);
	init(env);
	env->controller = NULL;
	if (SDL_IsGameController(0))
		env->controller = SDL_GameControllerOpen(0);
	render(env);
	loop(env, env->controller);
	return (0);
}

void	error(int err_nb)
{
	if (err_nb == SDL_ERR)
		ft_fprintf(2, "SDL_Error: %s\n", SDL_GetError());
	else if (err_nb == MALLOC_ERR)
		perror("Malloc error");
	else if (err_nb != 0)
		ft_fprintf(2, "Unknown error number: %d\n", err_nb);
	exit(EXIT_FAILURE);
}

void	exit_cleanly(t_env *env)
{
	env->controller ? SDL_GameControllerClose(env->controller) : (void)0;
	SDL_DestroyTexture(env->sdl.texture);
	SDL_DestroyRenderer(env->sdl.renderer);
	free(env->sdl.img);
	free(env->light_arr);
	free(env->objs_arr);
	SDL_DestroyWindow(env->sdl.win);
	SDL_Quit();
	exit(EXIT_SUCCESS);
}
