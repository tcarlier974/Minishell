/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:44:43 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:44:43 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	__child_redir_and_exec(t_minishell *shell, t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		__redir_fd(cmd->fd_in, STDIN_FILENO);
	if (cmd->fd_out != STDOUT_FILENO)
		__redir_fd(cmd->fd_out, STDOUT_FILENO);
	__close_other_fds(cmd);
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(shell, cmd);
		exit(shell->exit_status);
	}
	if (ft_strchr(cmd->args[0], '/'))
		exit(execute_file(shell, cmd->args[0], cmd->args));
	exit(__exec_path_cmd(shell, cmd));
}

static pid_t	__launch_empty_or_error(t_cmd *cmd, int status)
{
	pid_t	pid;

	(void)cmd;
	pid = fork();
	if (pid == 0)
		exit(status);
	return (pid);
}

pid_t	launch_process(t_minishell *shell, t_cmd *cmd)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0] || !cmd->args[0][0])
		return (__launch_empty_or_error(cmd, 0));
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		return (__launch_empty_or_error(cmd, 1));
	pid = fork();
	if (pid == 0)
		__child_redir_and_exec(shell, cmd);
	else if (pid > 0)
		cmd->pid = pid;
	else
	{
		print_error("fork", NULL, strerror(errno));
		shell->exit_status = 1;
		return (-1);
	}
	return (pid);
}

void	__setup_pipes(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		pipefd[2];

	tmp = cmd;
	while (tmp && tmp->next)
	{
		if (pipe(pipefd) == -1)
		{
			print_error("pipe", NULL, strerror(errno));
			return ;
		}
		if (tmp->fd_out == STDOUT_FILENO)
			tmp->fd_out = pipefd[1];
		else
			close(pipefd[1]);
		if (tmp->next->fd_in == STDIN_FILENO)
			tmp->next->fd_in = pipefd[0];
		else
			close(pipefd[0]);
		tmp = tmp->next;
	}
}

void	__wait_pipeline(t_minishell *shell, t_cmd *cmd, pid_t last_pid, int *la)
{
	t_cmd	*tmp;
	int		status;

	tmp = cmd;
	while (tmp)
	{
		if (tmp->pid > 0)
		{
			waitpid(tmp->pid, &status, 0);
			if (tmp->pid == last_pid)
			{
				if (WIFEXITED(status))
					shell->exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
				{
					shell->exit_status = 128 + WTERMSIG(status);
					if (WTERMSIG(status) == SIGSEGV)
						shell->exit_status = 139;
				}
			}
		}
		tmp = tmp->next;
	}
	if (*la)
		shell->exit_status = 0;
}
