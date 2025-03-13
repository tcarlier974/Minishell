/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:16:46 by tcarlier          #+#    #+#             */
/*   Updated: 2025/03/13 16:02:20 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**args;
	int		status;

	(void)argc;
	(void)argv;
	status = 0;
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		if (*line)
		{
			add_history(line);
			args = ft_split(line, ' ');
			args = parse(args, envp);
			status = ft_exec(args, envp);
			free(args);
		}
		free(line);
	}
	return (status);
}
