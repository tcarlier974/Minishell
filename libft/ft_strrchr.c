/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 17:16:55 by samberna          #+#    #+#             */
/*   Updated: 2024/11/05 17:17:00 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*last_occ;

	if (c == 0)
		return ((char *)s + ft_strlen(s));
	last_occ = NULL;
	while (*s)
	{
		if (*s == (char)c)
			last_occ = (char *)s;
		s++;
	}
	return (last_occ);
}
