/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:55:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/24 15:33:37 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_PARSER_H
# define RT_PARSER_H

# include "rtv1.h"

# define ENV	(1 << 0)
# define DISP	(1 << 1)
# define CAM	(1 << 2)
# define OBJS	(1 << 3)
# define LIGHTS	(1 << 4)

# define MAX_OBJS	0xFFFFF
# define MAX_LIGHTS	0xFFFFF

# define FILE_HEADER		"RICARD >>> PASTIS\n"
# define FILE_HEADER_LEN	18

int			is_comment(char *line);

void		parse_scene(t_env *env, char *filename);
void		parse_env(int fd, t_env *env);
void		parse_disp(int fd, t_env *env);
void		parse_camera(int fd, t_env *env);

void		parse_objects(int fd, t_env *env, char *line);
void		parse_lights(int fd, t_env *env, char *line);

int			tobj_parse(t_obj *obj, char *line, int *done,
					unsigned int line_nb);
t_obj		parse_sphere(int fd, unsigned int *line_nb);
t_obj		parse_plane(int fd, unsigned int *line_nb);
t_obj		parse_disk(int fd, unsigned int *line_nb);

void		parse_open_bracket(int fd, unsigned int *line_nb);
float		parse_f(char *str, unsigned int line_nb);
t_flt3		parse_f3(char *str, unsigned int line_nb);

#endif
