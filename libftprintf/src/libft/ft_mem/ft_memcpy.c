/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:45:06 by tuthayak          #+#    #+#             */
/*   Updated: 2024/12/19 16:45:06 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dst;
	const unsigned char	*src_ptr;

	if (!dest && !src)
		return (NULL);
	dst = (unsigned char *)dest;
	src_ptr = (const unsigned char *)src;
	while (n--)
	{
		*dst = *src_ptr;
		dst++;
		src_ptr++;
	}
	return (dest);
}
