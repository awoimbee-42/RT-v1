/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_disp_cam.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 15:18:42 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/22 18:50:52 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"

void	parse_env(int fd, t_env *env)
{
	char	*line;
	int		done;

	done = 0;
	line = NULL;
	env->bckgrnd_col = (t_fcolor){0.2, 0.2, 0.2};
	while (get_next_line(fd, &line) > 0 && ++env->keys_pressed)
	{
		if (!ft_strncmp(line, "\t.background_light", 18))
			env->bckgrnd_col = parse_f3(line + 18, env->keys_pressed);
		else if (*line != '\0')
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (done = 1))
			break ;
		ft_memdel((void**)&line);
	}
	if (line)
		ft_memdel((void**)&line);
	if (!done)
		msg_exit("Bad format in env, around line %d\n", &env->keys_pressed);
}

void	parse_disp(int fd, t_env *env)
{
	char	*line;
	int		done;

	done = 0;
	line = NULL;
	while (get_next_line(fd, &line) > 0 && ++env->keys_pressed)
	{
		if (!ft_strncmp(line, "\t.width", 7) && (done |= 0xF))
			env->disp.res.x = parse_f(line + 7, env->keys_pressed);
		else if (!ft_strncmp(line, "\t.height", 8) && (done |= 0xF0))
			env->disp.res.y = parse_f(line + 8, env->keys_pressed);
		else if (!ft_strncmp(line, "\t.fov", 5) && (done |= 0xF00))
			env->disp.tfov = parse_f(line + 5, env->keys_pressed);
		else if (*line != '\0')
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (done |= 0xF000))
			break ;
		ft_memdel((void**)&line);
	}
	if (line)
		ft_memdel((void**)&line);
	if (done != 0xFFFF)
		msg_exit("Bad format in display, around line %d\n", &env->keys_pressed);
	env->disp.aspect_ratio = (float)env->disp.res.x / env->disp.res.y;
	env->disp.tfov = tan(env->disp.tfov / 2 * M_PI / 180);
}

void	parse_camera(int fd, t_env *env)
{
	char	*line;
	int		done;

	done = 0;
	line = NULL;
	while (get_next_line(fd, &line) > 0 && ++env->keys_pressed)
	{
		if (!ft_strncmp(line, "\t.pos", 5) && (done |= 0xF))
			env->camera.org = parse_f3(line + 5, env->keys_pressed);
		else if (!ft_strncmp(line, "\t.dir", 5) && (done |= 0xF0))
			env->camera.dir = parse_f3(line + 5, env->keys_pressed);
		else if (*line != '\0')
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (done |= 0xF00))
			break ;
		ft_memdel((void**)&line);
	}
	if (line)
		ft_memdel((void**)&line);
	if (done != 0xFFF)
		msg_exit("Bad format in camera, around line %d\n", &env->keys_pressed);
}
