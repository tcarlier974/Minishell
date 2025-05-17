/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:20:16 by samberna          #+#    #+#             */
/*   Updated: 2024/11/05 17:20:27 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	i;
	size_t	swdst;
	size_t	swsrc;

	i = 0;
	if (dsize == 0)
		return (ft_strlen(src));
	swdst = ft_strlen(dst);
	swsrc = ft_strlen(src);
	if (dsize <= swdst)
		return (dsize + swsrc);
	while (src[i] && i < swsrc && i < (dsize - swdst - 1))
	{
		dst[swdst + i] = src[i];
		i++;
	}
	dst[swdst + i] = '\0';
	return (swdst + swsrc);
}
