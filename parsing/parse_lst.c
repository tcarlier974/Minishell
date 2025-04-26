/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:56:34 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 02:05:115 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token_type get_type(char *str)
{
	// < >> > >> | || CMD ARG
	if (is_ratiel(str) != 0)
		return (is_ratiel(str));
	else
		return (ARG);
}

t_token *tab_to_lst(char *line, t_data *data) 
{
	int i;
	t_token	*head;
	t_token	*tmp;
	
	head = data->token;
	tmp = head;
	i = 0;
	char **str = extract_cmd(line);
	while (str[i] != NULL)
	{
		tmp->type = get_type(str[i]);
		printf("%d\n", tmp->type);
		tmp->str = str[i];
		tmp = tmp->next;
		i++;
	}
	return (head);
}
