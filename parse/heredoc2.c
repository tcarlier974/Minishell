/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:26:18 by tcarlier          #+#    #+#             */
/*   Updated: 2025/05/20 21:27:51 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*__process_delimiter(char *delimiter)
{
	char	*processed;
	int		var[3];
	int		in_single_quote;
	int		in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	var[0] = 0;
	var[1] = 0;
	var[2] = ft_strlen(delimiter);
	processed = malloc(var[2] + 1);
	if (!processed)
		return (NULL);
	while (delimiter[var[0]])
	{
		if ((delimiter[var[0]] == '\'' && !in_double_quote)
			|| (delimiter[var[0]] == '"' && !in_single_quote))
		{
			__p_d_c(&in_single_quote, &in_double_quote, &var[0], delimiter);
		}
		else
			processed[var[1]++] = delimiter[var[0]++];
	}
	processed[var[1]] = '\0';
	return (processed);
}

static int	__should_expand_vars(char *original_delimiter)
{
	int	i;

	i = 0;
	while (original_delimiter[i])
	{
		if (original_delimiter[i] == '\'')
			return (0);
		i++;
	}
	return (1);
}

static void	__write_heredoc_line(int fd, char *line,
	t_minishell *shell, int expand)
{
	char	*expanded;

	if (expand && ft_strchr(line, '$'))
	{
		expanded = expand_word_with_quotes(shell, line);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

void	ifpido(int var[3], char *strs[3], t_minishell *shell)
{
	__handle_heredoc_signals();
	var[0] = open(strs[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (var[0] < 0)
		exit(1);
	while (1)
	{
		strs[0] = readline("heredoc> ");
		if (!strs[0] || !ft_strcmp(strs[0], strs[2]))
		{
			free(strs[0]);
			break ;
		}
		__write_heredoc_line(var[0], strs[0], shell, var[2]);
		free(strs[0]);
	}
	close(var[0]);
	free(strs[2]);
	free(strs[1]);
}

int	elsepid(int var[3], char *strs[3], t_minishell *shell, pid_t	pid)
{
	waitpid(pid, &var[1], 0);
	__restore_signals();
	free(strs[2]);
	if (WIFSIGNALED(var[1]))
	{
		shell->exit_status = 128 + WTERMSIG(var[1]);
		unlink(strs[1]);
		free(strs[1]);
		return (-1);
	}
	var[0] = open(strs[1], O_RDONLY);
	unlink(strs[1]);
	free(strs[1]);
	return (var[0]);
}
