/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:33:32 by awoimbee          #+#    #+#             */
/*   Updated: 2019/01/22 14:53:09 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double		ft_atoi_double(const char *nptr)
{
	short int		sign;
	double			result;
	float			i;

	result = 0;
	sign = 1;
	while (*nptr == ' ' || *nptr == '\n' || *nptr == '\t'
		|| *nptr == '\f' || *nptr == '\v' || *nptr == '\r')
		++nptr;
	if (*nptr == '-' && ++nptr)
		sign = -1;
	else if (*nptr == '+')
		++nptr;
	while ('0' <= *nptr && *nptr <= '9')
		result = result * 10 + (*nptr++ - 48);
	if (*nptr == '.' && ++nptr)
	{
		i = 1;
		while ('0' <= *nptr && *nptr <= '9' && (i *= 10))
			result += (float)(*nptr++ - 48) / i;
	}
	return (result * sign);
}
