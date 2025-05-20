/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:22:34 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:22:34 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

int	execute_file(t_minishell *shell, char *file_path, char **args)
{
	struct stat	st;
	char		*sh_argv[3];

	if (access(file_path, F_OK) != 0)
		return (print_error(file_path, NULL, strerror(errno)), 127);
	if (stat(file_path, &st) == 0 && S_ISDIR(st.st_mode))
		return (print_error(file_path, NULL, "Is a directory"), 126);
	if (access(file_path, X_OK) != 0)
		return (print_error(file_path, NULL, strerror(errno)), 126);
	if (stat(file_path, &st) != 0 || !S_ISREG(st.st_mode))
		return (print_error(file_path, NULL, "Not a regular file"), 126);
	execve(file_path, args, shell->env);
	if (errno == ENOEXEC)
	{
		sh_argv[0] = "sh";
		sh_argv[1] = file_path;
		sh_argv[2] = NULL;
		execve("/bin/sh", sh_argv, shell->env);
		return (print_error(file_path, NULL, "Exec format error"), 126);
	}
	print_error(file_path, NULL, strerror(errno));
	return (127);
}
