/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   heredoc.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: samberna <samberna@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/06/15 14:55:12 by jdupuis		   #+#	#+#			 */
/*   Updated: 2025/05/20 15:13:50 by samberna		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

static char		*g_heredoc_files[128];
static int		g_heredoc_count = 0;

static void	__store_heredoc_filename(char *filename)
{
	if (g_heredoc_count < 128)
		g_heredoc_files[g_heredoc_count++] = ft_strdup(filename);
}

static char	*__get_heredoc_filename(void)
{
	static int	counter = 0;
	char		filename[32];

	snprintf(filename, sizeof(filename), "/tmp/heredoc_%d", counter++);
	__store_heredoc_filename(filename);
	return (ft_strdup(filename));
}

static void	__handle_heredoc_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}

static void	__restore_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	__p_d_c(int *isq, int *idq, int *var, char *delimiter)
{
	if (delimiter[var[0]] == '\'' && !(*isq))
		*isq = 1;
	else if (delimiter[var[0]] == '\'' && *isq)
		*isq = 0;
	else if (delimiter[var[0]] == '"' && !(*idq))
		*idq = 1;
	else if (delimiter[var[0]] == '"' && *idq)
		*idq = 0;
	var[0]++;
}

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
// *strs[3]; [0] = line, [1] = heredoc_file, [2] = processed_delimiter
// var[3]; [0] = fd, [1] = status, [2] = expand_vars

int	__setup_heredoc(t_minishell *shell, char *delimiter)
{
	char	*strs[3];
	int		var[3];
	pid_t	pid;

	strs[1] = __get_heredoc_filename();
	if (!strs[1])
		return (-1);
	var[2] = __should_expand_vars(delimiter);
	strs[2] = __process_delimiter(delimiter);
	if (!strs[2])
	{
		free(strs[1]);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		return ((void)free(strs[1]), (void)free(strs[2]), -1);
	if (pid == 0)
	{
		ifpido(var, strs, shell);
		exit(0);
	}
	else
		return (elsepid(var, strs, shell, pid));
}

void	ifpido2(int var[3], char *strs[3], t_minishell *shell,
	char **delimiters)
{
	var[0] = open(strs[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (var[0] < 0)
		exit(1);
	while (*delimiters)
	{
		var[2] = __should_expand_vars(*delimiters);
		strs[2] = __process_delimiter(*delimiters);
		if (!strs[2])
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
		free(strs[2]);
		delimiters++;
	}
}

int	elsepid2(int var[3], char *strs[3], t_minishell *shell, pid_t	pid)
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

// *strs[3]; [0] = line, [1] = heredoc_file, [2] = processed_delimiter
// var[3]; [0] = fd, [1] = status, [2] = expand_vars

int	__setup_heredocs(t_minishell *shell, char **delimiters)
{
	char	*strs[3];
	int		var[3];
	pid_t	pid;

	strs[1] = __get_heredoc_filename();
	if (!strs[1])
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		__handle_heredoc_signals();
		ifpido2(var, strs, shell, delimiters);
		close(var[0]);
		free(strs[1]);
		exit(0);
	}
	else
		return (elsepid2(var, strs, shell, pid));
}
