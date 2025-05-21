/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:28:52 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/21 02:47:57 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	__clean_heredoc_delims(char **heredoc_delims, int *hdoc_count)
{
	int	i;

	i = 0;
	while (i < *hdoc_count)
	{
		free(heredoc_delims[i]);
		heredoc_delims[i] = NULL;
		i++;
	}
	*hdoc_count = 0;
}

static int	__handle_heredoc(t_heredoc_data *data)
{
	if (*(data->hdoc_count) < 127)
	{
		data->heredoc_delims[*(data->hdoc_count)] = ft_strdup(
				(*(data->token))->value);
		(*(data->hdoc_count))++;
	}
	if (!(*(data->token))->next || (*(data->token))->next->type != T_HEREDOC)
	{
		data->heredoc_delims[*(data->hdoc_count)] = NULL;
		if (data->current->fd_in != STDIN_FILENO)
			close(data->current->fd_in);
		data->current->fd_in = __setup_heredocs(data->shell,
				data->heredoc_delims);
		__clean_heredoc_delims(data->heredoc_delims, data->hdoc_count);
		if (data->current->fd_in < 0)
			return (1);
	}
	return (0);
}

static int	__process_heredoc(t_redir_data *d, char **h_delims, int *h_count)
{
	t_heredoc_data	hd_data;

	hd_data.shell = d->shell;
	hd_data.current = d->current;
	hd_data.token = d->token;
	hd_data.heredoc_delims = h_delims;
	hd_data.hdoc_count = h_count;
	if (!(*d->redir_error) && __handle_heredoc(&hd_data))
		return (__handle_redir_error(d->shell, "heredoc", d->redir_error));
	return (0);
}

int	__process_other_redir(t_redir_data *data, t_token_type redir_type)
{
	char	*filename;

	filename = process_concat_filename((*(data->token))->value);
	if (!(*(data->redir_error)) && __handle_redirection(
			data->shell, data->current, redir_type, filename))
		__handle_redir_error(data->shell, filename, data->redir_error);
	free(filename);
	return (0);
}

int	__handle_heredoc_redir(t_proc_red_data *d, t_redir_data *data)
{
	static char	*heredoc_delims[128];
	static int	hdoc_count;

	(void)d;
	return (__process_heredoc(data, heredoc_delims, &hdoc_count));
}
