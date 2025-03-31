/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:45:13 by tuthayak          #+#    #+#             */
/*   Updated: 2024/12/19 16:45:13 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dst;
	const unsigned char	*src_ptr;

	if (!dest && !src)
		return (NULL);
	dst = (unsigned char *)dest;
	src_ptr = (const unsigned char *)src;
	if (dst < src_ptr)
	{
		while (n--)
			*dst++ = *src_ptr++;
	}
	else
	{
		dst += n;
		src_ptr += n;
		while (n--)
			*(--dst) = *(--src_ptr);
	}
	return (dest);
}
