/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 15:28:36 by awoimbee          #+#    #+#             */
/*   Updated: 2019/06/23 21:48:24 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"
#include "../libft/libft.h"

/*
**	tobj_parse
**		because all objects have a lot in common.
**		returns 1 if somethig was retrieved, else 0
*/

int			tobj_parse(t_obj *obj, char *line, int *done,
							unsigned int line_nb)
{
	if (!ft_strncmp(line, "\t\t.diffuse", 10) && (*done |= 0xF))
		obj->diffuse = parse_f(line + 10, line_nb, 0.f);
	else if (!ft_strncmp(line, "\t\t.specular", 11) && (*done |= 0xF0))
		obj->specular = parse_f(line + 11, line_nb, 0.f);
	else if (!ft_strncmp(line, "\t\t.color", 8) && (*done |= 0xF00))
		obj->color = parse_f3(line + 8, line_nb, 0.f);
	else
		return (0);
	return (1);
}

static int	create_obj_arr(int fd, t_env *env, char *line)
{
	int		done;
	uint	linenb;

	linenb = env->keys;
	done = 0;
	env->objs_nb = 0;
	while (get_next_line(fd, &line) > 0 && ++linenb)
	{
		if (!ft_strcmp(line, "\tSphere") || !ft_strcmp(line, "\tPlane")
			|| !ft_strcmp(line, "\tDisk") || !ft_strcmp(line, "\tCylinder")
			|| !ft_strcmp(line, "\tCone") || !ft_strcmp(line, "\tTriangle"))
			env->objs_nb += 1;
		else if (!ft_strcmp(line, "}") && (done = 1))
			break ;
		ft_memdel((void*)&line);
	}
	ft_memdel((void*)&line);
	!done ? msg_exit("Objects : bad characters line: %d", &linenb) : (void)0;
	if (!(env->objs_arr = malloc(sizeof(t_obj) * (env->objs_nb + 1))))
		error(MALLOC_ERR);
	lseek(fd, 0, SEEK_SET);
	while(get_next_line(fd, &line) && ft_strncmp(line, "objects", 7))
		free(line);
	free(line);
	get_next_line(fd, &line);
	ft_memdel((void**)&line);
	return (env->objs_nb);
}

void		parse_objects(int fd, t_env *env, char *line, int obj_nb)
{
	int		done;

	done = 0;
	parse_open_bracket(fd, &env->keys);
	obj_nb = create_obj_arr(fd, env, line);
	env->objs_nb = obj_nb;
	while (get_next_line(fd, &line) > 0 && ++env->keys)
	{
		if (!ft_strcmp(line, "\tSphere") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_sphere(fd, &env->keys);
		else if (!ft_strcmp(line, "\tPlane") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_plane(fd, &env->keys);
		else if (!ft_strcmp(line, "\tDisk") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_disk(fd, &env->keys);
		else if (!ft_strcmp(line, "\tCylinder") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_cylinder(fd, &env->keys);
		else if (!ft_strcmp(line, "\tCone") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_cone(fd, &env->keys);
		else if (!ft_strcmp(line, "\tTriangle") && --obj_nb >= 0)
			env->objs_arr[obj_nb] = parse_triangle(fd, &env->keys);
		else if ((!ft_strcmp(line, "}") && (done = 1)) || 1)
			break ;
		ft_memdel((void*)&line);
	}
	ft_memdel((void*)&line);
	obj_nb != 0 ? msg_exit("%d objects more/less in def\n", &obj_nb) : (void)0;
	!done ? msg_exit("U didn't close bracket for objects!", 0) : (void)0;
}
