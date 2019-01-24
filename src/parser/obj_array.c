/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 15:28:36 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/23 00:53:23 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"

/*
**	tobj_parse
**		because all objects have a lot in common.
**		returns 1 if somethig was retrieved, else 0
*/

int			tobj_parse(t_obj *obj, char *line, int *done,
							unsigned int line_nb)
{
	if (!ft_strncmp(line, "\t\t.diffuse", 10) && (*done |= 0xF))
		obj->diffuse = parse_f(line + 10, line_nb);
	else if (!ft_strncmp(line, "\t\t.specular", 11) && (*done |= 0xF0))
		obj->specular = parse_f(line + 11, line_nb);
	else if (!ft_strncmp(line, "\t\t.color", 8) && (*done |= 0xF00))
		obj->color = parse_f3(line + 8, line_nb);
	else
		return (0);
	return (1);
}

static int	create_array(t_env *env, char *line)
{
	int		obj_nb;

	while (*line != '\0' && *line != '[')
		++line;
	if (!*line || !*++line)
		msg_exit("Bad format line %d\n", &env->keys_pressed);
	obj_nb = ft_atoi_mv(&line);
	if (ft_strncmp(line, "] :", 3) != 0 || obj_nb < 0 || obj_nb > MAX_OBJS)
		msg_exit("Bad character after size line %d\n", &env->keys_pressed);
	if (!(env->objs_arr = malloc(sizeof(t_obj) * (obj_nb + 1))))
		error(MALLOC_ERR);
	env->objs_arr[obj_nb] = (t_obj){NULL, NULL, 0, 0, (t_fcolor){0, 0, 0},
								{(struct s_sphere){(t_vec3){0, 0, 0}, 0}}};
	return (obj_nb);
}

void		parse_objects(int fd, t_env *env, char *line)
{
	int		done;
	int		obj_nb;

	done = 0;
	obj_nb = create_array(env, line);
	env->objs_nb = obj_nb;
	parse_open_bracket(fd, &env->keys_pressed);
	while (get_next_line(fd, &line) > 0 && ++env->keys_pressed)
	{
		if (!ft_strcmp(line, "\tSphere") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_sphere(fd, &env->keys_pressed);
		else if (!ft_strcmp(line, "\tPlane") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_plane(fd, &env->keys_pressed);
		else if (!ft_strcmp(line, "\tDisk") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_disk(fd, &env->keys_pressed);
		else if (!ft_strcmp(line, "}") && (done = 1))
			break ;
		else
			break ;
	}
	if (obj_nb != 0)
		msg_exit("Wrong number of objects in definition\n", 0);
	if (!done)
		msg_exit("Fuckyou, okay ?", 0); // mashallah
}
