/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:19:37 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:19:37 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__handle_redirection_main(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		handle_redir_token(tokens, &input[*i], T_APPEND);
		*i += 2;
	}
	else if (input[*i] == '>')
	{
		handle_redir_token(tokens, &input[*i], T_REDIR_OUT);
		*i += 1;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		handle_redir_token(tokens, &input[*i], T_HEREDOC);
		*i += 2;
	}
	else if (input[*i] == '<')
	{
		handle_redir_token(tokens, &input[*i], T_REDIR_IN);
		*i += 1;
	}
}

void	__handle_quotes_main(char *input, int *i, int *in_quote)
{
	char	quote_char;

	quote_char = input[*i];
	*in_quote = DOUBLE_QUOTE;
	if (quote_char == '\'')
		*in_quote = SINGLE_QUOTE;
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i])
	{
		*in_quote = NO_QUOTE;
		(*i)++;
	}
}

void	__process_token_main(t_token **t, char *input, int *i, int *in_quote)
{
	int		start;
	char	*value;

	start = *i;
	while (input[*i])
	{
		if (!*in_quote && (is_metachar(input[*i]) || is_whitespace(input[*i])))
			break ;
		if ((input[*i] == '\'' && *in_quote != DOUBLE_QUOTE)
			|| (input[*i] == '"' && *in_quote != SINGLE_QUOTE))
		{
			__handle_quotes_main(input, i, in_quote);
			continue ;
		}
		(*i)++;
	}
	if (*i > start)
	{
		value = strndup(input + start, *i - start);
		token_add_back(t, create_token(value, T_WORD));
	}
}

int	__is_whitespace_no_quote(char c, int in_quote)
{
	if (!in_quote && is_whitespace(c))
		return (1);
	return (0);
}

int	__is_pipe_no_quote(char c, int in_quote)
{
	if (!in_quote && c == '|')
		return (1);
	return (0);
}
