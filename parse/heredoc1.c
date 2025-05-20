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
