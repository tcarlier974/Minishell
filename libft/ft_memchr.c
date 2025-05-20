/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 17:16:54 by tristan           #+#    #+#             */
/*   Updated: 2024/11/04 11:58:28 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_p;
	size_t			i;

	s_p = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (s_p[i] == (unsigned char)c)
			return ((void *)s_p + i);
		i++;
	}
	return (NULL);
}
