/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:36:29 by tcarlier          #+#    #+#             */
/*   Updated: 2025/03/13 16:41:110 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_pipe(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (strcmp(args[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}

void	echo(char **args)
{
	int	i;
	int n;
	int p;
	
	i = 1;
	n = 0;
	p = 0;
	if (strcmp(args[1], "-n") == 0)
	{
		n = 1;
		i++;
	}
	if (check_pipe(args))
		p = 1;
	i = 1 + n;
	while (args[i])
	{
		ft_putstr_fd(args[i], fd);
		if (args[i + 1])
			ft_putstr_fd(" ", fd);
		i++;
	}
	if (n == 0)
		ft_putstr_fd("\n", fd);
}
