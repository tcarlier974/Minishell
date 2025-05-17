/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:51:41 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:51:41 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **env)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

char	*get_env_var(t_minishell *shell, char *var)
{
	size_t	len;
	int		i;

	if (!var)
		return (NULL);
	len = ft_strlen(var);
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], var, len)
			&& shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

// Helper to free env array
void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

static char	*__make_env_var(t_minishell *shell, char *var, char *value)
{
	char	*new_var;

	(void)shell; // Mark shell as unused to avoid warning

	new_var = NULL;
	if (value)
	{
		new_var = malloc(ft_strlen(var) + ft_strlen(value) + 2);
		if (new_var)
			ft_sprintf(new_var, "%s=%s", var, value);
	}
	else
		new_var = ft_strdup(var);
	return (new_var);
}

static int	__replace_env_var(t_minishell *shell, char *var, char *new_var, size_t len)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], var, len)
			&& (shell->env[i][len] == '=' || shell->env[i][len] == '\0'))
		{
			free(shell->env[i]);
			shell->env[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

int	set_env_var(t_minishell *shell, char *var, char *value)
{
	char	*new_var;
	size_t	len;
	int		count;
	char	**new_env;

	if (!is_valid_env_name(var))
		return (1);
	new_var = __make_env_var(shell, var, value);
	if (!new_var)
		return (1);
	len = ft_strlen(var);
	if (__replace_env_var(shell, var, new_var, len))
		return (0);
	count = 0;
	while (shell->env[count])
		count++;
	new_env = realloc(shell->env, sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return (1);
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	shell->env = new_env; // assign back to shell->env
	return (0);
}
