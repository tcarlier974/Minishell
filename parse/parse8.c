/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:29:17 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/20 20:30:00 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	__handle_other_redir(t_proc_red_data *d, t_redir_data *data)
{
	return (__process_other_redir(data, d->redir_type));
}

static int	__process_redirection_helper(t_proc_red_data *d)
{
	t_redir_data	data;

	d->redir_type = (*(d->token))->type;
	*(d->token) = (*(d->token))->next;
	if (!(*(d->token)) || (*(d->token))->type != T_WORD)
		return (__redir_syntax_error(d->shell, d->redir_type, d->cmd_list));
	data.shell = d->shell;
	data.current = d->current;
	data.token = d->token;
	data.cmd_list = d->cmd_list;
	data.redir_error = d->redir_error;
	if (d->redir_type == T_HEREDOC)
		return (__handle_heredoc_redir(d, &data));
	return (__handle_other_redir(d, &data));
}

int	__process_redirection(t_process_redir_args args)
{
	t_proc_red_data	d;

	d.shell = args.shell;
	d.current = args.current;
	d.token = args.token;
	d.cmd_list = args.cmd_list;
	d.redir_error = args.redir_error;
	return (__process_redirection_helper(&d));
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

t_cmd	*parse(t_minishell *shell, t_token *token)
{
	t_cmd			*cmd_list;
	t_cmd			*current;
	int				redir_error;
	t_process_args	args;
	int				result;

	__setup_parsing(&cmd_list, &current, &redir_error);
	if (__check_first_pipe(shell, token))
		return (NULL);
	while (token)
	{
		result = __handle_pipe_or_init(shell, &cmd_list, &current, &token);
		if (result == 1)
			return (NULL);
		if (result == 2)
		{
			redir_error = 0;
			if (!current)
				continue ;
		}
		args = (t_process_args){shell, current, &token, cmd_list, &redir_error};
		__process_token(&args);
		token = token->next;
	}
	return (cmd_list);
}
