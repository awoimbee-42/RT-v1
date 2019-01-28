/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_individual.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 18:33:26 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/28 17:02:23 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"
#include "../libft/libft.h"

t_obj		parse_cone(int fd, uint32_t *line_nb)
{
	char	*line;
	int	d;
	t_obj	cone;

	d = 0;
	line = NULL;
	cone.distfun = &dist_cone;
	cone.normfun = &norm_cone;
	while (get_next_line(fd, &line) > 0 && ++*line_nb)
	{
		if (!ft_strncmp(line, "\t\t.origin", 9) && (d |= 0xF000))
			cone.this.cone.org = parse_f3(line + 9, *line_nb);
		else if (!ft_strncmp(line, "\t\t.dir", 6) && (d |= 0xF0000))
			cone.this.cone.dir = parse_f3(line + 6, *line_nb);
		else if (!ft_strncmp(line, "\t\t.angle", 8) && (d |= 0xF00000))
			cone.this.cone.angle = parse_f(line + 8, *line_nb);
		else if (!tobj_parse(&cone, line, &d, *line_nb) && !is_comment(line))
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (d |= 0xF000000))
			break ;
		ft_memdel((void**)&line);
	}
	line ? ft_memdel((void**)&line) : 0;
	if (d != 0xFFFFFFF)
		msg_exit("Bad format in Cone, around line %d\n", line_nb);
	return (cone);
}

t_obj		parse_cylinder(int fd, unsigned int *line_nb)
{
	char	*line;
	int	d;
	t_obj	cylinder;

	d = 0;
	line = NULL;
	cylinder.distfun = &dist_cylinder;
	cylinder.normfun = &norm_cylinder;
	while (get_next_line(fd, &line) > 0 && ++*line_nb)
	{
		if (!ft_strncmp(line, "\t\t.origin", 9) && (d |= 0xF000))
			cylinder.this.cylinder.org = parse_f3(line + 9, *line_nb);
		else if (!ft_strncmp(line, "\t\t.end", 6) && (d |= 0xF0000))
			cylinder.this.cylinder.end = parse_f3(line + 6, *line_nb);
		else if (!ft_strncmp(line, "\t\t.radius", 9) && (d |= 0xF00000))
			cylinder.this.cylinder.radius = parse_f(line + 9, *line_nb);
		else if (!tobj_parse(&cylinder, line, &d, *line_nb) && !is_comment(line))
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (d |= 0xF000000))
			break ;
		ft_memdel((void**)&line);
	}
	line ? ft_memdel((void**)&line) : 0;
	if (d != 0xFFFFFFF)
		msg_exit("Bad format in Cylinder, around line %d\n", line_nb);
	return (cylinder);
}

t_obj		parse_sphere(int fd, unsigned int *line_nb)
{
	char	*line;
	int		d;
	t_obj	sphere;

	d = 0;
	line = NULL;
	sphere.distfun = &dist_sphere;
	sphere.normfun = &norm_sphere;
	while (get_next_line(fd, &line) > 0 && ++*line_nb)
	{
		if (!ft_strncmp(line, "\t\t.origin", 9) && (d |= 0xF000))
			sphere.this.sphere.orig = parse_f3(line + 9, *line_nb);
		else if (!ft_strncmp(line, "\t\t.radius", 9) && (d |= 0xF0000))
			sphere.this.sphere.radius = parse_f(line + 9, *line_nb);
		else if (!tobj_parse(&sphere, line, &d, *line_nb) && !is_comment(line))
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (d |= 0xF00000))
			break ;
		ft_memdel((void**)&line);
	}
	line ? ft_memdel((void**)&line) : 0;
	if (d != 0xFFFFFF)
		msg_exit("Bad format in Sphere, around line %d\n", line_nb);
	return (sphere);
}

t_obj		parse_plane(int fd, unsigned int *line_nb)
{
	char	*line;
	int		done;
	t_obj	plane;

	done = 0;
	line = NULL;
	plane.distfun = &dist_plane;
	plane.normfun = &norm_plane;
	while (get_next_line(fd, &line) > 0 && ++*line_nb)
	{
		if (!ft_strncmp(line, "\t\t.origin", 9) && (done |= 0xF000))
			plane.this.plane.orig = parse_f3(line + 9, *line_nb);
		else if (!ft_strncmp(line, "\t\t.normal", 9) && (done |= 0xF0000))
			plane.this.plane.norm = parse_f3(line + 9, *line_nb);
		else if (!tobj_parse(&plane, line, &done, *line_nb) && !is_comment(line))
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (done |= 0xF00000))
			break ;
		ft_memdel((void**)&line);
	}
	line ? ft_memdel((void**)&line) : 0;
	if (done != 0xFFFFFF)
		msg_exit("Bad format in plane, around line %d\n", line_nb);
	return (plane);
}

t_obj		parse_disk(int fd, unsigned int *line_nb)
{
	char	*line;
	int		done;
	t_obj	disk;

	done = 0;
	line = NULL;
	disk.distfun = &dist_disk;
	disk.normfun = &norm_plane;
	while (get_next_line(fd, &line) > 0 && ++*line_nb)
	{
		if (!ft_strncmp(line, "\t\t.origin", 9) && (done |= 0xF000))
			disk.this.disk.orig = parse_f3(line + 9, *line_nb);
		else if (!ft_strncmp(line, "\t\t.normal", 9) && (done |= 0xF0000))
			disk.this.disk.norm = parse_f3(line + 9, *line_nb);
		else if (!ft_strncmp(line, "\t\t.radius", 9) && (done |= 0xF00000))
			disk.this.disk.radius2 = pow(parse_f(line + 9, *line_nb), 2);
		else if (!tobj_parse(&disk, line, &done, *line_nb) && !is_comment(line))
			break ;
		if (line[ft_strlen(line) - 1] == ';' && (done |= 0xF000000))
			break ;
		ft_memdel((void**)&line);
	}
	line ? ft_memdel((void**)&line) : 0;
	if (done != 0xFFFFFFF)
		msg_exit("Bad format in disk, around line %d\n", line_nb);
	return (disk);
}
