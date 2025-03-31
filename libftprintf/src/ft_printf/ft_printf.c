/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 15:18:03 by tuthayak          #+#    #+#             */
/*   Updated: 2025/01/27 22:30:15 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

int	convert(const char c, va_list arg)
{
	int	count;

	count = 0;
	if (c == 'c')
		count += ft_putchar(va_arg(arg, int));
	else if (c == 's')
		count += ft_putstr(va_arg(arg, char *));
	else if (c == 'p')
		count += ft_putptr(va_arg(arg, unsigned long));
	else if (c == 'd')
		count += ft_putnbr(va_arg(arg, int));
	else if (c == 'i')
		count += ft_putnbr(va_arg(arg, int));
	else if (c == 'u')
		count += ft_putnbr(va_arg(arg, unsigned int));
	else if (c == 'x')
		count += ft_puthex(va_arg(arg, unsigned int), 'x');
	else if (c == 'X')
		count += ft_puthex(va_arg(arg, unsigned int), 'X');
	else if (c == '%')
		count += ft_putchar(c);
	return (count);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		count;
	va_list	arg;

	i = 0;
	count = 0;
	if (!str)
		return (-1);
	va_start(arg, str);
	while (str[i])
	{
		if (str[i] == '%')
			count += convert(str[++i], arg);
		else
			count += ft_putchar(str[i]);
		i++;
	}
	va_end(arg);
	return (count);
}
