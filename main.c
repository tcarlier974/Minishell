/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:16:46 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/09 23:48:52 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//https://github.com/Hqndler/42-minishell/tree/main

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	//char	**args;
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
			open_quote(line);
			printf("%d\n",get_number_segment(line));
			extract_cmd(line);
			return (0);
			parse(line, envp);
			//t_token *head = parse(line, envp);
			//printf("first elem parsed type %d\n", head->type);
			//status = ft_exec(args, envp);
			//free(args);
		}
		free(line);
	}
	return (status);
}
