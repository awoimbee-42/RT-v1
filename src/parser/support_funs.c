/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_funs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 15:09:52 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/22 20:21:55 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"

static char	*skip_whitespaces(char *str)
{
	while (*str == ' ' || *str == '\t')
		++str;
	return (str);
}

/*
**	parse_f3 takes " = {x, x, x}" | "={x, x, x}" as arg
**		line_nb is only given to be printed when errors occur
*/

t_flt3		parse_f3(char *str, unsigned int line_nb)
{
	t_flt3	res;

	str = skip_whitespaces(str);
	if (*str != '='
		|| *(str = skip_whitespaces(str + 1)) != '{'
		|| *(str = skip_whitespaces(str + 1)) == 0)
		msg_exit("error around '=' sign line %d!\n", &line_nb);
	res.x = ft_atoi_double_mv(&str);
	if (*(str = skip_whitespaces(str)) != ',' || !*++str)
		msg_exit("',' not found line %d\n", &line_nb);
	res.y = ft_atoi_double_mv(&str);
	if (*(str = skip_whitespaces(str)) != ',' || !*++str)
		msg_exit("',' not found line %d\n", &line_nb);
	res.z = ft_atoi_double_mv(&str);
	if (*(str = skip_whitespaces(str)) != '}')
		msg_exit("'}' not found line %d\n", &line_nb);
	return (res);
}

float		parse_f(char *str, unsigned int line_nb)
{
	float	res;

	if (*(str = skip_whitespaces(str)) != '=')
		msg_exit("'=' sign not found line %d!\n", &line_nb);
	++str;
	res = ft_atoi_double_mv(&str);
	if (*str != '\0' && *str != ';')
		msg_exit("Garbage at end of line %d\n", &line_nb);
	return (res);
}

void		parse_open_bracket(int fd, unsigned int *line_nb)
{
	char	*line;

	++*line_nb;
	if (get_next_line(fd, &line) <= 0 || ft_strcmp(line, "{") != 0)
		msg_exit("Bad openning bracket line %d\n", line_nb);
	ft_memdel((void**)&line);
}
