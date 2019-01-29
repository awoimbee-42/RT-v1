/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:38:31 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/29 16:29:00 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"

static void	scene_defaults(t_env *env)
{
	env->disp.res = (t_int2){800, 600};
	env->disp.aspect_ratio = 4. / 3.;
	env->disp.tfov = tan(75 / 2 * M_PI / 180);
	env->camera = (t_ray){(t_coords){0, 0, 0}, (t_vec3){0, 0, 1}};
	env->bckgrnd_col = (t_fcolor){0.2, 0.2, 0.2};
	env->objs_nb = 0;
	env->light_nb = 0;
}

static int	init_parser(int *done, t_env *env, const char *filename)
{
	int		fd;
	char	buff[FILE_HEADER_LEN];

	env->keys_pressed = 0;
	scene_defaults(env);
	*done = 0;
	if ((fd = open(filename, O_RDONLY)) == -1)
		msg_exit("could not open file \"%s\"", filename);
	if (read(fd, buff, FILE_HEADER_LEN) != FILE_HEADER_LEN
		|| ft_strncmp(buff, FILE_HEADER, FILE_HEADER_LEN))
		msg_exit("Incorrect file type. (file header not found)", NULL);
	return (fd);
}

static void	prt_scene_miss_info(const int done)
{
	if (!(done & 0xF))
		missing_clause_in_file("env", 1);
	if (!(done & 0xF0))
		missing_clause_in_file("display", 1);
	if (!(done & 0xF00))
		missing_clause_in_file("camera", 1);
	if (!(done & 0xF000))
		missing_clause_in_file("objects", 1);
	if (!(done & 0xF0000))
		missing_clause_in_file("lights", 1);
}

/*
**	Let's use env->keys_pressed as line nb because why not ?
*/

void		parse_scene(t_env *env, const char *filename)
{
	int		fd;
	char	*line;
	int		done;

	line = NULL;
	fd = init_parser(&done, env, filename);
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
		prt_scene_miss_info(done);
	env->keys_pressed = 0;
}
