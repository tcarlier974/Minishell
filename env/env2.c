/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:51:15 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:51:15 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_env_var(t_minishell *shell, char *var)
{
	size_t	len;
	int		i;
	int		j;

	if (!is_valid_env_name(var))
		return (1);
	len = ft_strlen(var);
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], var, len)
			&& shell->env[i][len] == '=')
		{
			free(shell->env[i]);
			j = i;
			while (shell->env[j])
			{
				shell->env[j] = shell->env[j + 1];
				j++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}

void	print_env(t_minishell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		write(1, shell->env[i], ft_strlen(shell->env[i]));
		write(1, "\n", 1);
		i++;
	}
}

int	is_valid_env_name(char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	i = 0;
	while (name[i])
	{
		if (i == 0 && !(ft_isalpha(name[i]) || name[i] == '_'))
			return (0);
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
