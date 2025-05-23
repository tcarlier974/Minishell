/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main8.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:19:32 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/23 20:43:33 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_pipe_no_quote(char c, int in_quote)
{
	if (!in_quote && c == '|')
		return (1);
	return (0);
}

t_token	*lexer(t_minishell *shell, char *input)
{
	t_token	*tokens;
	int		i;
	int		in_quote;

	if (!has_balanced_quotes(input))
		return (__handle_unbalanced_quotes(shell));
	tokens = NULL;
	i = 0;
	in_quote = NO_QUOTE;
	while (input[i])
	{
		if (__is_whitespace_no_quote(input[i], in_quote))
			i++;
		else if (__is_pipe_no_quote(input[i], in_quote))
			__handle_pipe(&tokens, &i);
		else if (__is_redirection_no_quote(input[i], in_quote))
		{
			if (!__lexer_handle_redirection(&tokens, input, &i, shell))
				return (NULL);
		}
		else
			__pt_main(&tokens, input, &i, &in_quote);
	}
	return (tokens);
}

void	__handle_quote_state(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !(*in_double))
	{
		if (*in_single)
			*in_single = 0;
		else
			*in_single = 1;
	}
	else if (c == '"' && !(*in_single))
	{
		if (*in_double)
			*in_double = 0;
		else
			*in_double = 1;
	}
}

void	__handle_single_in(t_token **tokens, char *input, int *i)
{
	handle_redir_token(tokens, &input[*i], T_REDIR_IN);
	*i += 1;
	if (__check_next_token(input, *i))
	{
		print_error("syntax error", NULL, "near '<'");
		*tokens = NULL;
	}
}
