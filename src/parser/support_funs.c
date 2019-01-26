/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   support_funs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 15:09:52 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/25 23:12:48 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
#include "parser.h"
#include "libft.h"

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

double		ft_atof_mv(char **nptr)
{
	short int		sign;
	double			result;
	float			i;

	result = 0;
	sign = 1;
	while (**nptr == ' ' || **nptr == '\n' || **nptr == '\t'
		|| **nptr == '\f' || **nptr == '\v' || **nptr == '\r')
		++*nptr;
	if (**nptr == '-' && ++*nptr)
		sign = -1;
	else if (**nptr == '+')
		++*nptr;
	while ('0' <= **nptr && **nptr <= '9')
		result = result * 10 + (*(*nptr)++ - 48);
	if (**nptr == '.' && ++*nptr)
	{
		i = 1;
		while ('0' <= **nptr && **nptr <= '9' && (i *= 10))
			result += (float)(*(*nptr)++ - 48) / i;
	}
	return (result * sign);
}


/*
**	msg_exit : a printf for crashing cleanly.
**	USAGE : msg_exit("error %[d, s]...", memory)
**		%s = string (char*)
**		%d = int
**	When %[...] is read,
**	 msg_exit will interpret memory as pointing to the specified datatype.
*/

static char	*skip_whitespaces(char *str)
{
	while (*str == ' ' || *str == '\t')
		++str;
	return (str);
}

int		is_comment(char *line)
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

t_flt3		parse_f3(char *str, unsigned int line_nb)
{
	t_flt3	res;

	str = skip_whitespaces(str);
	if (*str != '='
		|| *(str = skip_whitespaces(str + 1)) != '{'
		|| *(str = skip_whitespaces(str + 1)) == 0)
		msg_exit("error around '=' sign line %d!\n", &line_nb);
	res.x = ft_atof_mv(&str);
	if (*(str = skip_whitespaces(str)) != ',' || !*++str)
		msg_exit("',' not found line %d\n", &line_nb);
	res.y = ft_atof_mv(&str);
	if (*(str = skip_whitespaces(str)) != ',' || !*++str)
		msg_exit("',' not found line %d\n", &line_nb);
	res.z = ft_atof_mv(&str);
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
	res = ft_atof_mv(&str);
	if (!is_comment(str = skip_whitespaces(str)) && *str != ';')
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
