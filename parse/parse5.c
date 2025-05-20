/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 23:56:28 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/11 23:56:28 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	__handle_input_redir(t_minishell *shell, t_cmd *current,
		t_token_type redir_type, char *fl)
{
	char	*delimiters[2];

	if (current->fd_in != STDIN_FILENO)
		close(current->fd_in);
	if (redir_type == T_REDIR_IN)
	{
		current->fd_in = open(fl, O_RDONLY);
		if (current->fd_in < 0)
			return (1);
	}
	else if (redir_type == T_HEREDOC)
	{
		delimiters[0] = fl;
		delimiters[1] = NULL;
		current->fd_in = __setup_heredocs(shell, delimiters);
		if (current->fd_in < 0)
			return (1);
	}
	return (0);
}

static int	__handle_output_redir(t_cmd *current, t_token_type redir_type,
		char *fl)
{
	int	flags;

	if (current->fd_out != STDOUT_FILENO)
		close(current->fd_out);
	flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (redir_type == T_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	current->fd_out = open(fl, flags, 0644);
	if (current->fd_out < 0)
		return (1);
	return (0);
}

int	__handle_redirection(t_minishell *shell, t_cmd *current,
		t_token_type redir_type, char *fl)
{
	(void)shell;
	if (redir_type == T_REDIR_IN || redir_type == T_HEREDOC)
		return (__handle_input_redir(shell, current, redir_type, fl));
	else
		return (__handle_output_redir(current, redir_type, fl));
}

int	__redir_syntax_error(t_minishell *shell, t_token_type redir_type,
		t_cmd *cmd_list)
{
	(void)shell;
	if (redir_type == T_APPEND)
		print_error("syntax error", NULL, "near '>>'");
	else if (redir_type == T_REDIR_OUT)
		print_error("syntax error", NULL, "near '>'");
	else if (redir_type == T_HEREDOC)
		print_error("syntax error", NULL, "near '<<'");
	else
		print_error("syntax error", NULL, "near '<'");
	free_cmds(cmd_list);
	return (1);
}

int	__handle_redir_error(t_minishell *shell, char *filename,
		int *redir_error)
{
	print_error(filename, NULL, strerror(errno));
	*redir_error = 1;
	shell->exit_status = 1;
	return (1);
}
