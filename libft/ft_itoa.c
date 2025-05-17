/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:02:04 by samberna          #+#    #+#             */
/*   Updated: 2024/11/06 13:39:48 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	nblen(long n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long	nb;
	char	*dst;
	int		i;

	if (n == 0)
		return (ft_strdup("0"));
	dst = (char *)malloc(sizeof(char) * (nblen((long)n) + 1));
	if (!dst)
		return (NULL);
	nb = (long)n;
	if (nb < 0)
	{
		dst[0] = '-';
		nb *= -1;
	}
	if (!dst)
		return (NULL);
	i = nblen((long)n) - 1;
	while (nb > 0)
	{
		dst[i--] = (nb % 10) + '0';
		nb /= 10;
	}
	dst[nblen((long)n)] = '\0';
	return (dst);
}
