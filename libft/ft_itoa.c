/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:28:24 by tristan           #+#    #+#             */
/*   Updated: 2024/11/04 11:58:19 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int	count_nb(int n)
{
	long int	i;
	int			count;

	count = 0;
	i = n;
	if (n < 0)
	{
		count++;
		i = -i;
	}
	while (i > 9)
	{
		i = i / 10;
		count++;
	}
	return (count + 1);
}

void	ft_sign(int *sign, long int *i, char **res)
{
	if (*i < 0)
	{
		*i = *i * -1;
		*res[0] = '-';
		*sign = 1;
	}
}

char	*ft_itoa(int n)
{
	char		*res;
	long int	i;
	int			k;
	int			sign;

	res = (char *)malloc(count_nb(n) * sizeof(char) + 1);
	if (!res)
		return (NULL);
	i = n;
	k = 0;
	sign = 0;
	ft_sign(&sign, &i, &res);
	while (i > 9)
	{
		res[count_nb(n) - k - 1] = i % 10 + '0';
		i = i / 10;
		k++;
	}
	res[sign] = i + '0';
	res[count_nb(n)] = '\0';
	return (res);
}
