/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f.c                                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:54:03 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/09 21:37:35 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_strstrlen(char **ptr)
{
	int len = 0;

	while (ptr[len] != NULL)
		len++;
	return len;
}

int	is_space(char c)
{
	return(c == ' ');
}