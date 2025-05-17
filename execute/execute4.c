/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:44:55 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:44:55 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	__close_and_reset_fd(int *fd, int std)
{
	if (*fd != std)
	{
		close(*fd);
		*fd = -1;
	}
}

static void	__update_last_empty(t_cmd *cur, int *last_was_empty)
{
	if (!cur->args || !cur->args[0] || !cur->args[0][0])
		*last_was_empty = 1;
	else
		*last_was_empty = 0;
}

static void	__exec_pipeline(t_minishell *shell, t_cmd *cmd)
{
	t_cmd	*cur;
	pid_t	last_pid;
	int		last_was_empty;
	pid_t	pid;

	cur = cmd;
	last_pid = -1;
	last_was_empty = 0;
	__setup_pipes(cmd);
	while (cur)
	{
		pid = launch_process(shell, cur);
		if (pid > 0)
		{
			last_pid = pid;
			__close_and_reset_fd(&cur->fd_in, STDIN_FILENO);
			__close_and_reset_fd(&cur->fd_out, STDOUT_FILENO);
		}
		__update_last_empty(cur, &last_was_empty);
		cur = cur->next;
	}
	__wait_pipeline(shell, cmd, last_pid, &last_was_empty);
}

static void	__exec_single_cmd(t_minishell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		shell->exit_status = 1;
	else if (is_builtin(cmd->args[0]))
		__exec_single_builtin(shell, cmd);
	else
	{
		pid = launch_process(shell, cmd);
		if (pid > 0)
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
	}
}

void	execute(t_minishell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		shell->exit_status = 0;
		return ;
	}
	shell->cmd = cmd;
	if (cmd->next)
		__exec_pipeline(shell, cmd);
	else
		__exec_single_cmd(shell, cmd);
	__close_fds(shell, cmd);
	free_cmds(shell->cmd);
	shell->cmd = NULL;
	if (shell->tokens != NULL)
		free_tokens(shell->tokens);
	shell->tokens = NULL;
}
