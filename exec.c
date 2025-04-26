/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:02:59 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 02:29:08 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

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
	if (pid == 0)
	{
		if (execve(args[0], args, envp) < 0)
		{
			perror("minishell");
			exit(1);
		}
	}
	else
		waitpid(pid, &status, 0);
	return (status);
}
