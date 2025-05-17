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

void	__check_quotes(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

int	__is_entirely_in_single_quotes(char *str)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*str)
	{
		__check_quotes(*str, &in_single_quote, &in_double_quote);
		str++;
	}
	return (in_single_quote);
}

void	expand_variables(t_minishell *shell, t_token *token)
{
	char	*expanded;

	while (token)
	{
		if (token->type == T_WORD)
		{
			if (!__is_entirely_in_single_quotes(token->value))
			{
				expanded = expand_word_with_quotes(shell, token->value);
				free(token->value);
				token->value = expanded;
			}
		}
		token = token->next;
	}
}

char	*__append_char(char *result, char c)
{
	char	tmp[2];
	char	*new_result;

	tmp[0] = c;
	tmp[1] = '\0';
	new_result = join_strs(result, tmp);
	free(result);
	return (new_result);
}

char	*__handle_quotes(char *result, char c, int *quote_flag)
{
	*quote_flag = !(*quote_flag);
	return (__append_char(result, c));
}
