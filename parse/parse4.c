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

char	*process_concat_filename(char *filename)
{
	char	*processed;
	int		len;

	len = strlen(filename);
	processed = malloc(len + 2);
	if (!processed)
		return (NULL);
	if (!__process_concat_loop(filename, processed, len))
	{
		free(processed);
		return (NULL);
	}
	return (processed);
}

int	__process_concat_loop(char *filename, char *processed, int len)
{
	int	read_pos;
	int	write_pos;
	int	a;
	int	in_double_quote;

	read_pos = 0;
	write_pos = 0;
	a = 0;
	in_double_quote = 0;
	while (read_pos < len)
	{
		if (__is_quote(filename[read_pos], a, in_double_quote))
			__process_quotes(filename[read_pos++], &a, &in_double_quote);
		else
			__copy_char(processed, filename[read_pos++], &write_pos);
	}
	processed[write_pos] = '\0';
	return (1);
}

int	__is_quote(char c, int in_single_quote, int in_double_quote)
{
	if (c == '\'' && !in_double_quote)
		return (1);
	if (c == '"' && !in_single_quote)
		return (1);
	return (0);
}

t_cmd	*__create_new_cmd(t_cmd *cmd_list, t_cmd **current)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
	{
		free_cmds(cmd_list);
		return (NULL);
	}
	new->args = NULL;
	new->fd_in = STDIN_FILENO;
	new->fd_out = STDOUT_FILENO;
	new->next = NULL;
	new->pid = -1;
	if (!cmd_list)
		cmd_list = new;
	else
		(*current)->next = new;
	*current = new;
	return (cmd_list);
}

char	**__add_argument(char **args, char *value, t_cmd *cmd_list)
{
	int		count;
	char	**new_args;

	count = 0;
	if (args)
		while (args[count])
			count++;
	new_args = realloc(args, sizeof(char *) * (count + 2));
	if (!new_args)
	{
		free_cmds(cmd_list);
		return (NULL);
	}
	new_args[count] = strdup(value);
	new_args[count + 1] = NULL;
	return (new_args);
}
