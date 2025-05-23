/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:19:49 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:19:49 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_redirection_no_quote(char c, int in_quote)
{
	if (!in_quote && (c == '>' || c == '<'))
		return (1);
	return (0);
}

t_token	*__handle_unbalanced_quotes(t_minishell *shell)
{
	print_error("syntax error", NULL, "unbalanced quotes");
	shell->exit_status = 1;
	return (NULL);
}

void	__handle_pipe(t_token **tokens, int *i)
{
	token_add_back(tokens, create_token(ft_strdup("|"), T_PIPE));
	(*i)++;
}

static int	__check_triple_redirection(char *input, int i, t_minishell *shell,
	t_token **tokens)
{
	if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>')
	{
		print_error("syntax error", NULL, "near '>'");
		shell->exit_status = 2;
		free_tokens(*tokens);
		return (0);
	}
	return (1);
}

int	__lexer_handle_redirection(t_token **tokens, char *input, int *i,
	t_minishell *shell)
{
	if (!__check_triple_redirection(input, *i, shell, tokens))
		return (0);
	__handle_redirection_main(tokens, input, i);
	if (!*tokens)
	{
		shell->exit_status = 2;
		return (0);
	}
	return (1);
}
