/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse9.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:54:36 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/23 22:10:19 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	__parse_loop(t_parse_data *data)
{
	int	result;

	while (*(data->token))
	{
		result = __parse_token(data);
		if (result == 0)
			return (0);
		if (result == 2)
			continue ;
		if (!(*(data->token)))
			break ;
		*(data->token) = (*(data->token))->next;
	}
	return (1);
}

t_cmd	*parse(t_minishell *shell, t_token *token)
{
	t_cmd			*cmd_list;
	t_cmd			*current;
	int				redir_error;
	t_parse_data	data;

	__setup_parsing(&cmd_list, &current, &redir_error);
	if (__check_first_pipe(shell, token))
		return (NULL);
	data.shell = shell;
	data.cmd_list = &cmd_list;
	data.current = &current;
	data.token = &token;
	data.redir_error = &redir_error;
	if (!__parse_loop(&data))
		return (NULL);
	return (cmd_list);
}
