/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:57:10 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:57:10 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_echo_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(t_cmd *cmd)
{
	int	n_flag;
	int	i;
	int	first;

	n_flag = 0;
	i = 1;
	while (cmd->args[i] && ft_echo_flag(cmd->args[i]))
	{
		n_flag = 1;
		i++;
	}
	first = 1;
	while (cmd->args[i])
	{
		if (!first)
			write(1, " ", 1);
		write(1, cmd->args[i], ft_strlen(cmd->args[i]));
		first = 0;
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}

int	ft_cd(t_minishell *shell, t_cmd *cmd)
{
	char	*path;
	char	cwd[MAX_PATH];

	if (count_args(cmd->args) > 2)
		return (print_error("cd", NULL, "too many arguments"), 1);
	path = cmd->args[1];
	if (!path || !ft_strcmp(path, "~"))
	{
		path = get_env_var(shell, "HOME");
		if (!path)
			return (print_error("cd", NULL, "HOME not set"), 1);
	}
	if (chdir(path) == -1)
		return (print_error("cd", path, strerror(errno)), 1);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_var(shell, "PWD", cwd);
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[MAX_PATH];

	if (getcwd(cwd, sizeof(cwd)))
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		return (0);
	}
	print_error("pwd", NULL, strerror(errno));
	return (1);
}

int	ft_cmp_env(const void *a, const void *b)
{
	const char	*s1 = *(const char **)a;
	const char	*s2 = *(const char **)b;

	return (ft_strcmp(s1, s2));
}
