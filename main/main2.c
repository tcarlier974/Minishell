/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:19:25 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:19:25 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	length_within_quote(char quote_char, char *str, int pos)
{
	int	i;

	i = 2;
	pos++;
	while (str[pos] && str[pos] != quote_char)
	{
		pos++;
		i++;
	}
	return (i);
}

void	handle_single_quote(int *in_quote)
{
	if (*in_quote == SINGLE_QUOTE)
		*in_quote = NO_QUOTE;
	else
		*in_quote = SINGLE_QUOTE;
}

void	handle_double_quote(int *in_quote)
{
	if (*in_quote == DOUBLE_QUOTE)
		*in_quote = NO_QUOTE;
	else
		*in_quote = DOUBLE_QUOTE;
}

void	handle_quotes(char *input, int *i, int *in_quote)
{
	if (input[*i] == '\'' && *in_quote != DOUBLE_QUOTE)
	{
		handle_single_quote(in_quote);
		(*i)++;
	}
	else if (input[*i] == '"' && *in_quote != SINGLE_QUOTE)
	{
		handle_double_quote(in_quote);
		(*i)++;
	}
	else
		(*i)++;
}

char	*get_redir_value(char *input, t_token_type type)
{
	char	*value;

	if (type == T_APPEND || type == T_HEREDOC)
		value = ft_strndup(input, 2);
	else
		value = ft_strndup(input, 1);
	return (value);
}
