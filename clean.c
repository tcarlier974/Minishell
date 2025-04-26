/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:05:43 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/27 01:41:38 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	clean_token(t_token *token)
{
	t_token	*tmp;
	
	tmp = token;
	while (tmp)
	{
		token = token->next;
		free(tmp);
		tmp = token;
	}
}

void	clean_extract(char **str)
{
	int i;
	
	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	clean(t_data **data)
{
	clean_token((*data)->token);
	free((*data)->pwd);
	free((*data));
}