/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:19:56 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:19:56 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_filename(char *filename)
{
	char	*processed;
	int		read_pos;
	int		write_pos;
	int		in_single;
	int		in_double;

	read_pos = 0;
	write_pos = 0;
	in_single = 0;
	in_double = 0;
	processed = malloc(ft_strlen(filename) + 1);
	if (!processed)
		return (NULL);
	while (filename[read_pos])
	{
		if ((filename[read_pos] == '\'' && !in_double)
			|| (filename[read_pos] == '"' && !in_single))
			__handle_quote_state(filename[read_pos], &in_single, &in_double);
		else
			processed[write_pos++] = filename[read_pos];
		read_pos++;
	}
	processed[write_pos] = '\0';
	return (processed);
}

void	__process_builtin_args(t_cmd *cmd)
{
	int		i;
	char	*stripped;

	i = 0;
	while (cmd->args[i])
	{
		stripped = strip_surrounding_quotes(cmd->args[i]);
		free(cmd->args[i]);
		cmd->args[i] = stripped;
		i++;
	}
}

void	__process_non_builtin_args(t_cmd *cmd)
{
	int		i;
	char	*processed;

	i = 0;
	while (cmd->args[i])
	{
		processed = process_filename(cmd->args[i]);
		free(cmd->args[i]);
		cmd->args[i] = processed;
		i++;
	}
}

void	process_cmd_args(t_cmd *cmd)
{
	if (!cmd || !cmd->args)
		return ;
	if (cmd->args[0] && strcmp(cmd->args[0], "echo") == 0)
	{
		process_echo_args(cmd);
		return ;
	}
	if (cmd->args[0] && is_builtin(cmd->args[0]))
		__process_builtin_args(cmd);
	else
		__process_non_builtin_args(cmd);
}

void	__free_and_replace_input(char **input, char *remaining_cmd)
{
	free(*input);
	*input = strdup(remaining_cmd);
}
