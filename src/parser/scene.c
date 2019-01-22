/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:38:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/22 20:21:46 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"

/*
**	file format:
**		env
**			.background_light = {.., .., ..};
**		display
**			.width = 1000
**			.height = 500
**			.fov = 75;
**		camera
**			.pos = {0., 0., 0.}
**			.dir = {1., 2., 0.};
**		objects[X] :
**		{
**			Sphere
**				.origin = {0., 0., 0.}
**				.radius = 5.
**				.color = {0.2, 0.4, 0.8};
**			Plane
**				...
**		}
**		lights[x] :
**		{
**			light
**				.position = {_, _, _}
**				.intensity = {_, _, _};
**		}
*/

/*
**	Let's use env->keys_pressed as line nb because why not ?
*/

void		parse_scene(t_env *env)
{
	int		fd;
	char	*line;
	int		done;

	env->keys_pressed = 0;
	done = 0;
	line = NULL;
	if ((fd = open("scene.rt", O_RDONLY)) == -1)
		msg_exit("could not open scene.rt", 0);
	while (get_next_line(fd, &line) > 0 && ++env->keys_pressed)
	{
		if (!ft_strcmp(line, "env") && (done |= 0xF))
			parse_env(fd, env);
		else if (!ft_strcmp(line, "display") && (done |= 0xF0))
			parse_disp(fd, env);
		else if (!ft_strcmp(line, "camera") && (done |= 0xF00))
			parse_camera(fd, env);
		else if (!ft_strncmp(line, "objects", 7) && (done |= 0xF000))
			parse_objects(fd, env, line);
		else if (!ft_strncmp(line, "lights", 6) && (done |= 0xF0000))
			parse_lights(fd, env, line);
		ft_memdel((void**)&line);
	}
	if (line)
		ft_memdel((void**)&line);
	if (done != 0xFFFFF)
		msg_exit("missing information in scene.rt", 0);
	env->keys_pressed = 0;
}
