/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:27:37 by tristan           #+#    #+#             */
/*   Updated: 2024/11/04 11:59:43 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if ((!haystack || !needle) && len == 0)
		return (NULL);
	if (*needle == '\0')
		return ((char *)haystack);
	if ((int)len < 0)
		return (ft_strnstr(haystack, needle, ft_strlen((char *)haystack)));
	if (len == 1 && haystack[0] == needle[0])
		return ((char *)haystack);
	while ((char)haystack[i] != '\0' && i < len)
	{
		j = 0;
		while ((char)haystack[i + j] == (char)needle[j] && i + j < len)
		{
			if (j == ft_strlen((char *)needle) - 1)
				return ((char *)(haystack + i));
			j++;
		}
		i++;
	}
	return (NULL);
}
