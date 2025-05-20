/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:19:11 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:19:11 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redir_token(t_token **tokens, char *input, t_token_type type)
{
	char	*value;

	value = get_redir_value(input, type);
	token_add_back(tokens, create_token(value, type));
}

static void	__update_quote_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
		*in_single = !(*in_single);
	else if (c == '"' && !(*in_single))
		*in_double = !(*in_double);
}

static void	__copy_char_if_not_quote(char *result, char c, int *j)
{
	result[*j] = c;
	(*j)++;
}

char	*process_echo_quotes(char *arg)
{
	char	*result;
	int		i;
	int		j;
	int		in_single;
	int		in_double;

	result = malloc(ft_strlen(arg) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (i < (int)ft_strlen(arg))
	{
		if ((arg[i] == '\'' && !in_double) || (arg[i] == '"' && !in_single))
			__update_quote_state(arg[i], &in_single, &in_double);
		else
			__copy_char_if_not_quote(result, arg[i], &j);
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	process_echo_args(t_cmd *cmd)
{
	int		i;
	char	*processed;

	if (!cmd || !cmd->args || ft_strcmp(cmd->args[0], "echo") != 0)
		return ;
	i = 1;
	while (cmd->args[i])
	{
		processed = process_echo_quotes(cmd->args[i]);
		free(cmd->args[i]);
		cmd->args[i] = processed;
		i++;
	}
}
