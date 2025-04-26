/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:01:33 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 02:29:42 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_words(char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	if (s[i - 1] != c)
		count++;
	return (count);
}

char	*ft_strndup(char *s1, int len)
{
	char	*s2;
	size_t	i;

	i = 0;
	s2 = (char *)malloc((len + 2) * sizeof(char));
	if (!s2)
		return (NULL);
	while (s1[i] && (int)i < len)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}


char	**ft_split(char const *s1, char c)
{
	char	**res;
	int		s;
	int		e;
	int		k;

	k = 0;
	s = 0;
	if (!s1)
		return (NULL);
	res = (char **)malloc((count_words((char *)s1, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (k < count_words((char *)s1, c) && s1[s])
	{
		while (s1[s] == c && s1[s])
			s++;
		e = s;
		while (s1[e] != c && s1[e])
			e++;
		res[k] = ft_strndup((char *)s1 + s, e - s);
		k++;
		s = e;
	}
	res[k] = 0;
	return (res);
}

char	**ft_splitdouble(char const *s1, char c)
{
	char	**res;
	int		s;
	int		e;
	int		k;

	k = 0;
	s = 0;
	if (!s1)
		return (NULL);
	res = (char **)malloc((count_words((char *)s1, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (k < count_words((char *)s1, c) && s1[s])
	{
		while (s1[s] == c && s1[s])
			s++;
		e = s;
		while (s1[e] != c && s1[e])
			e++;
		res[k] = ft_strndup((char *)s1 + s, e - s);
		k++;
		s = e;
	}
	res[k] = 0;
	return (res);
}
