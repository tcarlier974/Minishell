/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:44:32 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:44:32 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

void	execute_builtin(t_minishell *shell, t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "echo"))
		shell->exit_status = ft_echo(cmd);
	else if (!ft_strcmp(cmd->args[0], "cd"))
		shell->exit_status = ft_cd(shell, cmd);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		shell->exit_status = ft_pwd();
	else if (!ft_strcmp(cmd->args[0], "export"))
		shell->exit_status = ft_export(shell, cmd);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		shell->exit_status = ft_unset(shell, cmd);
	else if (!ft_strcmp(cmd->args[0], "env"))
		shell->exit_status = ft_env(shell);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		shell->exit_status = ft_exit(shell, cmd);
}

void	__close_other_fds(t_cmd *cmd)
{
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		if (tmp != cmd)
		{
			if (tmp->fd_in != STDIN_FILENO && tmp->fd_in >= 0)
				close(tmp->fd_in);
			if (tmp->fd_out != STDOUT_FILENO && tmp->fd_out >= 0)
				close(tmp->fd_out);
		}
		tmp = tmp->next;
	}
}

void	__exec_path_cmd(t_minishell *shell, t_cmd *cmd)
{
	char		*path_env;
	char		*dir;
	char		*saveptr;
	char		fullpath[MAX_PATH];
	struct stat	st;

	// Check if it's a relative path like ./something
	if (cmd->args[0][0] == '.' && cmd->args[0][1] == '/')
	{
		if (access(cmd->args[0], F_OK) == 0)
		{
			if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode))
			{
				print_error(cmd->args[0], NULL, "Is a directory");
				exit(126);
			}
			if (access(cmd->args[0], X_OK) != 0)
			{
				print_error(cmd->args[0], NULL, strerror(errno));
				exit(126);
			}
			execve(cmd->args[0], cmd->args, shell->env);
			print_error(cmd->args[0], NULL, strerror(errno));
			exit(127);
		}
		print_error(cmd->args[0], NULL, "No such file or directory");
		exit(127);
	}

	// Original code for PATH search
	path_env = get_env_var(shell, "PATH");
	if (!path_env || !path_env[0])
	{
		print_error(cmd->args[0], NULL, "No such file or directory");
		exit(127);
	}
	
	// PATH search logic
	path_env = ft_strdup(path_env);
	saveptr = NULL;
	dir = strtok_r(path_env, ":", &saveptr);
	while (dir)
	{
		snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, cmd->args[0]);
		if (access(fullpath, X_OK) == 0)
			execve(fullpath, cmd->args, shell->env);
		dir = strtok_r(NULL, ":", &saveptr);
	}
	free(path_env);
	print_error(cmd->args[0], NULL, "command not found");
	free_cmds(shell->cmd);
	free_tokens(shell->tokens); // Free command list before exit
	free_env(shell->env); // Free environment variables
	exit(127);
}

void	__redir_fd(int fd, int std)
{
	if (fd != std)
	{
		if (fd < 0)
		{
			if (std == STDIN_FILENO)
				close(STDIN_FILENO);
			exit(1);
		}
		dup2(fd, std);
		close(fd);
	}
}
