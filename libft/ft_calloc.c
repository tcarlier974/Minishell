/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 23:55:44 by tristan           #+#    #+#             */
/*   Updated: 2024/11/04 11:58:05 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;

	if (count == SIZE_MAX || size == SIZE_MAX
		|| ((long)size < 0 && (long)count < 0) || count * size >= 2147483647)
		return (NULL);
	if (count == 0 || size == 0)
	{
		res = malloc(1);
		return (res);
	}
	res = (void *)malloc(count * size);
	if (!res)
		return (NULL);
	ft_bzero(res, count * size);
	return (res);
}
