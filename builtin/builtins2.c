/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:57:25 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:57:25 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_print_export_line(char *env)
{
	char	*eq;

	eq = ft_strchr(env, '=');
	if (eq)
	{
		write(1, "declare -x ", 11);
		write(1, env, eq - env);
		write(1, "=\"", 2);
		write(1, eq + 1, ft_strlen(eq + 1));
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, "declare -x ", 11);
		write(1, env, ft_strlen(env));
		write(1, "\n", 1);
	}
}

static void	ft_print_export_sorted(t_minishell *shell)
{
	int		count;
	int		i;
	char	**sorted;

	count = 0;
	while (shell->env[count])
		count++;
	sorted = malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (i < count)
	{
		sorted[i] = shell->env[i];
		i++;
	}
	sorted[count] = NULL;
	qsort(sorted, count, sizeof(char *), ft_cmp_env);
	i = 0;
	while (i < count)
	{
		ft_print_export_line(sorted[i]);
		i++;
	}
	free(sorted);
}

static int	__export_handle_arg(t_minishell *shell, char *arg)
{
	char	*equal;
	char	*name;
	int		ret;

	ret = 0;
	equal = ft_strchr(arg, '=');
	if (equal)
		name = ft_strndup(arg, equal - arg);
	else
		name = ft_strdup(arg);
	if (!is_valid_env_name(name))
	{
		print_error("export", name, "not a valid identifier");
		free(name);
		return (1);
	}
	if (equal)
		set_env_var(shell, name, equal + 1);
	else if (!get_env_var(shell, name))
		set_env_var(shell, name, NULL);
	free(name);
	return (ret);
}

int	ft_export(t_minishell *shell, t_cmd *cmd)
{
	int	i;
	int	ret;

	if (!cmd->args[1])
		return (ft_print_export_sorted(shell), 0);
	i = 1;
	ret = 0;
	while (cmd->args[i])
	{
		if (__export_handle_arg(shell, cmd->args[i]))
			return (1);
		i++;
	}
	return (ret);
}

int	ft_unset(t_minishell *shell, t_cmd *cmd)
{
	int	i;

	if (!cmd->args[1])
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		if (!is_valid_env_name(cmd->args[i]))
		{
			print_error("unset", cmd->args[i], "not a valid identifier");
			return (1);
		}
		unset_env_var(shell, cmd->args[i]);
		i++;
	}
	return (0);
}
