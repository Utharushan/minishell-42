/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:41:40 by tuthayak          #+#    #+#             */
/*   Updated: 2024/12/19 16:41:40 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

int	is_space(char c)
{
	return ((9 <= c && c <= 13) || c == 32);
}

int	ft_atoi(const char *str)
{
	int	atoi;
	int	i;
	int	sign;

	atoi = 0;
	i = 0;
	sign = 1;
	while (is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (48 <= str[i] && str[i] <= 57)
	{
		atoi = atoi * 10 + (str[i] - 48);
		i++;
	}
	return (sign * atoi);
}
