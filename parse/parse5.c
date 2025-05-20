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

int	__handle_redirection(t_minishell *shell, t_cmd *current,
		t_token_type redir_type, char *fl)
{
	int		flags;
	char	*delimiters[2];

	(void)shell;
	if (redir_type == T_REDIR_IN || redir_type == T_HEREDOC)
	{
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
	}
	else
	{
		if (current->fd_out != STDOUT_FILENO)
			close(current->fd_out);
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		if (redir_type == T_APPEND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		current->fd_out = open(fl, flags, 0644);
		if (current->fd_out < 0)
			return (1);
	}
	return (0);
}

static int	__redir_syntax_error(t_minishell *shell, t_token_type redir_type, t_cmd *cmd_list)
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

static int	__handle_redir_error(t_minishell *shell, char *filename, int *redir_error)
{
	print_error(filename, NULL, strerror(errno));
	*redir_error = 1;
	shell->exit_status = 1;
	return (1);
}

int	__process_redirection(t_minishell *shell, t_cmd *current, t_token **token, t_cmd *cmd_list,
		int *redir_error)
{
	t_token_type	redir_type;
	char			*filename;
	static char		*heredoc_delims[128] = {NULL};
	static int		hdoc_count = 0;

	redir_type = (*token)->type;
	*token = (*token)->next;
	if (!(*token) || (*token)->type != T_WORD)
		return (__redir_syntax_error(shell, redir_type, cmd_list));
	if (redir_type == T_HEREDOC)
	{
		if (hdoc_count < 127)
			heredoc_delims[hdoc_count++] = ft_strdup((*token)->value);
		if (!(*token)->next || (*token)->next->type != T_HEREDOC)
		{
			heredoc_delims[hdoc_count] = NULL;
			if (!(*redir_error))
			{
				if (current->fd_in != STDIN_FILENO)
					close(current->fd_in);
				current->fd_in = __setup_heredocs(shell, heredoc_delims);
				if (current->fd_in < 0)
					__handle_redir_error(shell, "heredoc", redir_error);
			}
			for (int i = 0; i < hdoc_count; i++)
			{
				free(heredoc_delims[i]);
				heredoc_delims[i] = NULL;
			}
			hdoc_count = 0;
		}
		return (0);
	}
	hdoc_count = 0;
	filename = process_concat_filename((*token)->value);
	if (!(*redir_error))
	{
		if (__handle_redirection(shell, current, redir_type, filename))
			__handle_redir_error(shell, filename, redir_error);
	}
	free(filename);
	return (0);
}

t_cmd	*__hpacc(t_minishell *shell, t_cmd **cmd_list, t_cmd **current,
			t_token **token)
{
	(void)shell;
	*cmd_list = __create_new_cmd(*cmd_list, current);
	if (!(*cmd_list))
		return (NULL);
	if ((*token)->type == T_PIPE)
	{
		*token = (*token)->next;
		return (*cmd_list);
	}
	return (*cmd_list);
}
