/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:16:46 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 23:41:35 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

//https://github.com/Hqndler/42-minishell/tree/main

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	*data;
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
			if (open_quote(line))
				continue ;
			printf("%d\n",get_number_segment(line));
			init(&data, line);
			data->token = tab_to_lst(line, data);
			//[0] = '\'' [-1] = '\'' = cut and send
			//[0] = '\"' [-1] = '\"' cut, replace env var calls and send
		}
		free(line);
	}
	return (status);
}
