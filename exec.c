/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:02:59 by tcarlier          #+#    #+#             */
/*   Updated: 2025/05/02 19:22:42 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

// > >> < << | cmd arg

t_token	*recup_heredoc(t_data *data)
{
	t_token	*head;
	char	*line;
	
	head = data->token;
	while (head->next)
	{
		if (head->type == HEREDOC)
		{
			line = readline("heredoc> ");
			while (ft_strcmp(line, head->next->str))
			{
				write(head->pipe[1], line, ft_strlen(line));
				write(head->pipe[1], "\n", 1);
				free(line);
				line = readline("heredoc> ");
			}
			free(line);
		}
		head = head->next;
	}
	return (data->token);
}

void	ft_setup_exec(t_data *data, char **envp)
{
	(void)envp;
	t_token *head = data->token;
	while (head->next)
	{
		printf("%s@@\n", head->str);
		if (head->next != NULL)
			head = head->next;
	}
}

int	ft_exec(char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	status = 0;
	if (pid < 0)
	{
		perror("minishell");
		return (1);
	}
	// close(pipe[0]);
	// dup2(pipe[1], STDOUT_FILENO);
	// close(pipe[1]);
	if (pid == 0)
	{
		// if (is_builtin(args[0]) < 0)
		// {
		// 	perror("minishell");
		// 	exit(EXIT_FAILURE);
		// }
		if (execve(args[0], args, envp) < 0)
		{
			perror("minishell");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);
	return (0);
}
