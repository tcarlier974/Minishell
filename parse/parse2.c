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

char	*__handle_dollar(t_minishell *shell, char *result, char *word, int *i)
{
	char	*expanded;
	char	*new_result;

	expanded = handle_dollar_sign(shell, word, i);
	new_result = join_strs(result, expanded);
	free(result);
	free(expanded);
	return (new_result);
}

char	*expand_word_with_quotes(t_minishell *shell, char *word)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	result = strdup("");
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (word[i])
	{
		if (word[i] == '\'' && !in_double_quote)
			result = __handle_quotes(result, word[i++], &in_single_quote);
		else if (word[i] == '"' && !in_single_quote)
			result = __handle_quotes(result, word[i++], &in_double_quote);
		else if (word[i] == '$' && !in_single_quote)
			result = __handle_dollar(shell, result, word, &i);
		else
			result = __append_char(result, word[i++]);
	}
	return (result);
}

char	*handle_dollar_sign(t_minishell *shell, char *str, int *i)
{
	(*i)++;
	if (str[*i] == '?')
	{
		(*i)++;
		return (handle_exit_status(shell, strdup("")));
	}
	if (!isalpha(str[*i]) && str[*i] != '_')
	{
		return (strdup("$"));
	}
	return (handle_env_var(shell, str, i, strdup("")));
}

char	*handle_exit_status(t_minishell *shell, char *result)
{
	char	exit_status[12];
	char	*new_result;

	sprintf(exit_status, "%d", shell->exit_status);
	new_result = join_strs(result, exit_status);
	free(result);
	return (new_result);
}

char	*__append_dollar(char *result)
{
	char	*new_result;

	new_result = join_strs(result, "$");
	free(result);
	return (new_result);
}
