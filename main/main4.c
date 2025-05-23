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

int	__check_next_token(char *input, int i)
{
	while (input[i] && is_whitespace(input[i]))
		i++;
	if (input[i] == '>' || input[i] == '<')
		return (1);
	if (input[i] == '\0')
		return (1);
	return (0);
}

void	__handle_triple_out(t_token **tokens, char *input, int *i)
{
	(void)input;
	print_error("syntax error", NULL, "near '>'");
	token_add_back(tokens, create_token(ft_strdup(">>>"), T_REDIR_OUT));
	*i += 3;
}

void	__handle_double_out(t_token **tokens, char *input, int *i)
{
	handle_redir_token(tokens, &input[*i], T_APPEND);
	*i += 2;
	if (__check_next_token(input, *i))
	{
		print_error("syntax error", NULL, "near '>>'");
		*tokens = NULL;
	}
}

void	__handle_single_out(t_token **tokens, char *input, int *i)
{
	handle_redir_token(tokens, &input[*i], T_REDIR_OUT);
	*i += 1;
	if (__check_next_token(input, *i))
	{
		print_error("syntax error", NULL, "near '>'");
		*tokens = NULL;
	}
}

void	__handle_heredoc(t_token **tokens, char *input, int *i)
{
	handle_redir_token(tokens, &input[*i], T_HEREDOC);
	*i += 2;
	if (__check_next_token(input, *i))
	{
		print_error("syntax error", NULL, "near '<<'");
		*tokens = NULL;
	}
}
