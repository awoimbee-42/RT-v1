/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 16:11:46 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/26 17:47:51 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"

static t_light	parse_light(int fd, unsigned int *line_nb)
{
	char	*line;
	int		done;
	t_light	light;

	done = 0;
	line = NULL;
	while (get_next_line(fd, &line) > 0 && ++*line_nb)
	{
		if (!ft_strncmp(line, "\t\t.position", 11) && (done |= 0xF))
			light.pos = parse_f3(line + 11, *line_nb);
		else if (!ft_strncmp(line, "\t\t.intensity", 12) && (done |= 0xF0))
			light.intensity = parse_f3(line + 12, *line_nb);
		else if (*line != '\0')
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (done |= 0xF00))
			break ;
		ft_memdel((void**)&line);
	}
	if (line)
		ft_memdel((void**)&line);
	if (done != 0xFFF)
		msg_exit("Bad format in Light, around line %d\n", line_nb);
	return (light);
}

static int		create_array(t_env *env, char *line)
{
	int		lights_nb;

	while (*line != '\0' && *line != '[')
		++line;
	if (!*line || !*++line)
		msg_exit("Bad format line %d\n", &env->keys_pressed);
	lights_nb = ft_atoi_mv(&line);
	if (ft_strncmp(line, "] :", 3) != 0 || lights_nb < 0
		|| lights_nb > MAX_LIGHTS)
		msg_exit("Bad character after size line %d\n", &env->keys_pressed);
	if (!(env->light_arr = malloc(sizeof(t_light) * (lights_nb + 1))))
		error(MALLOC_ERR);
	env->light_arr[lights_nb] = (t_light){(t_coords){0, 0, 0},
										(t_fcolor){0, 0, 0}};
	return (lights_nb);
}

void			parse_lights(int fd, t_env *env, char *line)
{
	int		done;
	int		lights_nb;

	done = 0;
	lights_nb = create_array(env, line);
	env->light_nb = lights_nb;
	parse_open_bracket(fd, &env->keys_pressed);
	while (get_next_line(fd, &line) > 0 && ++env->keys_pressed)
	{
		if (!ft_strcmp(line, "\tLight") && --lights_nb >= 0)
			env->light_arr[lights_nb] = parse_light(fd, &env->keys_pressed);
		else if (!ft_strcmp(line, "}") && (done = 1))
			break ;
		else
			break ;
	}
	if (lights_nb != 0)
		msg_exit("Wrong number of lights in definition\n", 0);
	if (!done)
		msg_exit("MADLAD", 0);
}
