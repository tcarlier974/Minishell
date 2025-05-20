/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:44:50 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:44:50 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__close_fds(t_minishell *shell, t_cmd *cmd)
{
	t_cmd	*tmp;

	(void)shell;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->fd_in != STDIN_FILENO && tmp->fd_in >= 0)
			close(tmp->fd_in);
		if (tmp->fd_out != STDOUT_FILENO && tmp->fd_out >= 0)
			close(tmp->fd_out);
		tmp = tmp->next;
	}
}

static void	__restore_std(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}

static void	__dup_and_close(int oldfd, int newfd)
{
	dup2(oldfd, newfd);
	close(oldfd);
}

static void	__dup_redir(int fd, int std, int *err)
{
	if (fd != std)
	{
		if (fd >= 0)
			__dup_and_close(fd, std);
		else
			*err = 1;
	}
}

void	__exec_single_builtin(t_minishell *shell, t_cmd *cmd)
{
	int	stdin_copy;
	int	stdout_copy;
	int	redir_err;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	redir_err = 0;
	__dup_redir(cmd->fd_in, STDIN_FILENO, &redir_err);
	if (!redir_err)
		__dup_redir(cmd->fd_out, STDOUT_FILENO, &redir_err);
	if (redir_err)
		shell->exit_status = 1;
	else
		execute_builtin(shell, cmd);
	__restore_std(stdin_copy, stdout_copy);
}
