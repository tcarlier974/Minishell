/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:19:17 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/20 14:59:37 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_parse_error(t_minishell *shell)
{
	if (shell->cmd)
	{
		free_cmds(shell->cmd);
		shell->cmd = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
}

void	cleanup(t_minishell *shell)
{
	if (shell->env)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
	if (shell->cmd)
	{
		free_cmds(shell->cmd);
		shell->cmd = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, env);
	shell_loop(&shell);
	cleanup(&shell);
	clear_history();
	rl_clear_history();
	return (shell.exit_status);
}

void	cpfd(const char *s, int fd)
{
	ft_putstr_fd(s, fd);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}
