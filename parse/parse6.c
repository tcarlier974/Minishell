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

void	__process_token(t_process_args *args)
{
	t_process_redir_args	redir_args;

	if ((*args->token)->type == T_WORD && !(*args->redir_error))
		args->current->args = __add_argument(args->current->args,
				(*args->token)->value, args->cmd_list);
	else if ((*args->token)->type == T_REDIR_IN
		|| (*args->token)->type == T_REDIR_OUT
		|| (*args->token)->type == T_APPEND
		|| (*args->token)->type == T_HEREDOC)
	{
		redir_args.shell = args->shell;
		redir_args.current = args->current;
		redir_args.token = args->token;
		redir_args.cmd_list = args->cmd_list;
		redir_args.redir_error = args->redir_error;
		if (__process_redirection(redir_args))
			*args->redir_error = 1;
	}
}

int	__check_first_pipe(t_minishell *shell, t_token *token)
{
	if (token && token->type == T_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		cleanup_parse_error(shell);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}

static int	__check_pipe_at_end(t_minishell *shell, t_token *token,
				t_cmd *cmd_list)
{
	if (token->type == T_PIPE && !token->next)
	{
		cpfd("minishell: syntax error near unexpected token `|'\n", 2);
		shell->exit_status = 2;
		if (cmd_list)
			free_cmds(cmd_list);
		cleanup_parse_error(shell);
		return (1);
	}
	return (0);
}

void	__setup_parsing(t_cmd **cmd_list, t_cmd **current, int *redir_error)
{
	*cmd_list = NULL;
	*current = NULL;
	*redir_error = 0;
}

int	__handle_pipe_or_init(t_minishell *shell, t_cmd **cmd_list,
				t_cmd **current, t_token **token)
{
	if (__check_pipe_at_end(shell, *token, *cmd_list))
		return (1);
	if (!*current || (*token)->type == T_PIPE)
	{
		*cmd_list = __hpacc(shell, cmd_list, current, token);
		if (!*cmd_list)
			return (1);
		return (2);
	}
	return (0);
}
