/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:16:46 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 02:17:005 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

//https://github.com/Hqndler/42-minishell/tree/main

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_data	*data = NULL;
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
			printf("%d\n",get_number_segment(line));
			init(data, line);
			write(1, "ok", 2);
			data->token = tab_to_lst(line, data);
			return (0);
			//[0] = '\'' [-1] = '\'' = cut and send
			//[0] = '\"' [-1] = '\"' cut, replace env var calls and send
		}
		free(line);
	}
	return (status);
}
