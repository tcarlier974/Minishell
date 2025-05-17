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

char	*__append_env_value(char *result, char *value)
{
	char	*new_result;

	new_result = join_strs(result, value);
	free(result);
	return (new_result);
}

void	__extract_var_name(char *str, int *i, char *var_name)
{
	int	j;

	j = 0;
	while (isalnum(str[*i]) || str[*i] == '_')
	{
		var_name[j++] = str[(*i)++];
	}
	var_name[j] = '\0';
}

char	*handle_env_var(t_minishell *shell, char *str, int *i, char *result)
{
	char	var_name[256];
	char	*value;

	if (str[*i] == '?')
	{
		(*i)++;
		return (handle_exit_status(shell, result));
	}
	__extract_var_name(str, i, var_name);
	if (var_name[0] == '\0')
		return (__append_dollar(result));
	value = get_env_var(shell, var_name);
	if (value)
		return (__append_env_value(result, value));
	return (result);
}

void	__process_quotes(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

void	__copy_char(char *processed, char c, int *write_pos)
{
	processed[(*write_pos)++] = c;
}
