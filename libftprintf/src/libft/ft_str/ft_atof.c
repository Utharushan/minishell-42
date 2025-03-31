/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:56:14 by tuthayak          #+#    #+#             */
/*   Updated: 2025/02/21 21:56:14 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

static int	is_space(char c)
{
	return ((9 <= c && c <= 13) || c == 32);
}

static double	parse_fraction(const char *str, int *i)
{
	double	fraction;
	double	divisor;

	fraction = 0.0;
	divisor = 10.0;
	while (str[*i] >= '0' && str[*i] <= '9')
	{
		fraction += (str[*i] - '0') / divisor;
		divisor *= 10.0;
		(*i)++;
	}
	return (fraction);
}

double	ft_atof(const char *str)
{
	double	result;
	int		i;
	int		sign;

	result = 0.0;
	i = 0;
	sign = 1;
	while (is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10.0 + (str[i] - '0');
		i++;
	}
	if (str[i] == '.')
	{
		i++;
		result += parse_fraction(str, &i);
	}
	return (sign * result);
}
