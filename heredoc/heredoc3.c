/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:26:20 by tcarlier          #+#    #+#             */
/*   Updated: 2025/05/23 19:44:39 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// *strs[3]; [0] = line, [1] = heredoc_file, [2] = processed_delimiter
// var[3]; [0] = fd, [1] = status, [2] = expand_vars

int	__setup_heredoc(t_minishell *shell, char *delimiter)
{
	char	*strs[3];
	int		var[3];
	pid_t	pid;

	strs[1] = __get_heredoc_filename();
	if (!strs[1])
		return (-1);
	var[2] = __should_expand_vars(delimiter);
	strs[2] = __process_delimiter(delimiter);
	if (!strs[2])
	{
		free(strs[1]);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		return ((void)free(strs[1]), (void)free(strs[2]), -1);
	if (pid == 0)
	{
		ifpido(var, strs, shell);
		exit(0);
	}
	else
		return (elsepid(var, strs, shell, pid));
}

void	ifpido2(int var[3], char *strs[3], t_minishell *shell,
	char **delimiters)
{
	var[0] = open(strs[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (var[0] < 0)
		exit(1);
	while (*delimiters)
	{
		var[2] = __should_expand_vars(*delimiters);
		strs[2] = __process_delimiter(*delimiters);
		if (!strs[2])
			exit(1);
		while (1)
		{
			strs[0] = readline("heredoc> ");
			if (!strs[0] || !ft_strcmp(strs[0], strs[2]))
			{
				free(strs[0]);
				break ;
			}
			__write_heredoc_line(var[0], strs[0], shell, var[2]);
			free(strs[0]);
		}
		free(strs[2]);
		delimiters++;
	}
}

int	elsepid2(int var[3], char *strs[3], t_minishell *shell, pid_t	pid)
{
	waitpid(pid, &var[1], 0);
	__restore_signals();
	if (WIFSIGNALED(var[1]))
	{
		shell->exit_status = 128 + WTERMSIG(var[1]);
		unlink(strs[1]);
		free(strs[1]);
		return (-1);
	}
	var[0] = open(strs[1], O_RDONLY);
	unlink(strs[1]);
	free(strs[1]);
	return (var[0]);
}

// *strs[3]; [0] = line, [1] = heredoc_file, [2] = processed_delimiter
// var[3]; [0] = fd, [1] = status, [2] = expand_vars

int	__setup_heredocs(t_minishell *shell, char **delimiters)
{
	char	*strs[3];
	int		var[3];
	pid_t	pid;

	strs[1] = __get_heredoc_filename();
	if (!strs[1])
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		__handle_heredoc_signals();
		ifpido2(var, strs, shell, delimiters);
		close(var[0]);
		free(strs[1]);
		exit(0);
	}
	else
		return (elsepid2(var, strs, shell, pid));
}
