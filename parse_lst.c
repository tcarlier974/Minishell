/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:56:34 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/23 23:07:57 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type get_type(char *str)
{
	// < >> > >> | || CMD ARG
	
}

t_token *tab_to_lst(char *line) 
{
	t_token *HEAD = NULL;
	t_token *prev = NULL;
	int i = 0;
	char **str = extract_cmd(line);
	while (str[i] != NULL)
	{
		t_token *tmp = (t_token*)malloc(sizeof(t_token*));
		tmp->type = get_type(str[i]);
	}
	return (NULL);
}