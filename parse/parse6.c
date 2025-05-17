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

void	__process_token(t_minishell *shell, t_cmd *current, t_token **token,
			t_cmd *cmd_list, int *redir_error)
{
	if ((*token)->type == T_WORD && !(*redir_error))
		current->args = __add_argument(current->args, (*token)->value,
				cmd_list);
	else if ((*token)->type == T_REDIR_IN || (*token)->type == T_REDIR_OUT
		|| (*token)->type == T_APPEND || (*token)->type == T_HEREDOC)
	{
		if (__process_redirection(shell, current, token, cmd_list, redir_error))
			*redir_error = 1;
	}
}

t_cmd	*parse(t_minishell *shell, t_token *token)
{
	t_cmd	*cmd_list;
	t_cmd	*current;
	int		redir_error;

	cmd_list = NULL;
	current = NULL;
	redir_error = 0;
	while (token)
	{
		if (!current || token->type == T_PIPE)
		{
			cmd_list = __handle_pipe_and_create_cmd(shell, &cmd_list, &current,
					&token);
			if (!cmd_list)
				return (NULL);
			redir_error = 0;
			if (!current)
				continue ;
		}
		__process_token(shell, current, &token, cmd_list, &redir_error);
		token = token->next;
	}
	return (cmd_list);
}
