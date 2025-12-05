/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_funs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 15:09:52 by awoimbee          #+#    #+#             */
/*   Updated: 2019/03/20 00:59:14 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"
#include "libft.h"

char		*skip_whitespaces(char *str)
{
	while (*str == ' ' || *str == '\t')
		++str;
	return (str);
}

int			is_comment(char *line)
{
	line = skip_whitespaces(line);
	if (*line == '\0' || *line == '#')
		return (1);
	return (0);
}

/*
**	parse_f3 takes " = {x, x, x}" | "={x, x, x}" as arg
**		line_nb is only given to be printed when errors occur
*/

t_flt3		parse_f3(char *str, unsigned int line_nb, float min)
{
	t_flt3	res;

	str = skip_whitespaces(str);
	if (*str != '='
		|| *(str = skip_whitespaces(str + 1)) != '{'
		|| *(str = skip_whitespaces(str + 1)) == 0)
		ft_msg_exit("error around '=' sign line %d!\n", &line_nb);
	res.x = ft_atof_mv(&str);
	if (res.x < min || *(str = skip_whitespaces(str)) != ',' || !*++str)
		ft_msg_exit("',' not found or bad value line %d\n", &line_nb);
	res.y = ft_atof_mv(&str);
	if (res.y < min || *(str = skip_whitespaces(str)) != ',' || !*++str)
		ft_msg_exit("',' not found or bad value line %d\n", &line_nb);
	res.z = ft_atof_mv(&str);
	if (res.z < min || *(str = skip_whitespaces(str)) != '}')
		ft_msg_exit("'}' not found or bad value line %d\n", &line_nb);
	return (res);
}

float		parse_f(char *str, unsigned int line_nb, float min)
{
	float	res;

	if (*(str = skip_whitespaces(str)) != '=')
		ft_msg_exit("'=' sign not found line %d!\n", &line_nb);
	++str;
	res = ft_atof_mv(&str);
	if (res < min)
		ft_msg_exit("What exactly are you trying to do here ? line %d", &line_nb);
	if (!is_comment(str = skip_whitespaces(str)) && *str != ';')
		ft_msg_exit("Garbage at end of line %d\n", &line_nb);
	return (res);
}

void		parse_open_bracket(int fd, unsigned int *line_nb)
{
	char	*line;

	++*line_nb;
	if (get_next_line(fd, &line) <= 0 || ft_strcmp(line, "{") != 0)
		ft_msg_exit("Bad openning bracket line %d\n", line_nb);
	ft_memdel((void**)&line);
}
