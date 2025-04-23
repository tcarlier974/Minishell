/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:16:46 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/22 17:27:12 by tcarlier         ###   ########.fr       */
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
	(void)envp;

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
			//[0] = '\'' [-1] = '\'' = cut and send
			//[0] = '\"' [-1] = '\"' cut, replace env var calls and send
		}
		free(line);
	}
	return (status);
}
