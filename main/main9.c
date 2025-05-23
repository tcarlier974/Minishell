/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main9.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:34:32 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/23 20:39:49 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	__handle_redirection_out(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '>' && input[*i + 1] == '>' && input[*i + 2] == '>')
		__handle_triple_out(tokens, input, i);
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		__handle_double_out(tokens, input, i);
		if (*tokens == NULL)
			return ;
	}
	else if (input[*i] == '>')
	{
		__handle_single_out(tokens, input, i);
		if (*tokens == NULL)
			return ;
	}
}

void	__handle_redirection_main(t_token **tokens, char *input, int *i)
{
	if (input[*i] == '>')
		__handle_redirection_out(tokens, input, i);
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		__handle_heredoc(tokens, input, i);
		if (*tokens == NULL)
			return ;
	}
	else if (input[*i] == '<')
	{
		__handle_single_in(tokens, input, i);
		if (*tokens == NULL)
			return ;
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

void	__pt_main(t_token **t, char *input, int *i, int *in_quote)
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
		value = ft_strndup(input + start, *i - start);
		token_add_back(t, create_token(value, T_WORD));
	}
}

int	__is_whitespace_no_quote(char c, int in_quote)
{
	if (!in_quote && is_whitespace(c))
		return (1);
	return (0);
}
