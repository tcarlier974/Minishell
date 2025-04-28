/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:02:59 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/28 01:05:54 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

// > >> < << | cmd arg

void	ft_setup_exec(t_data *data, char **envp)
{
	(void)envp;
	t_token *head = data->token;
	while (head->next)
	{
		printf("%s @@ %d @@ %d %d\n", head->str, head->type, head->pipe[0], head->pipe[1]);
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
