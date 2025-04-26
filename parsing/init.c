/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:21:00 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 02:39:40 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_t_token(t_token *token, char *arg)
{
	t_token	*tmp;
	t_token	*tmp2;
	int i;
	
	i = 0;
	token->str = NULL;
	token->type = 0;
	token->prev = NULL;
	token->next = malloc(sizeof(t_token));
	tmp = token->next;
	tmp->prev = token;
	tmp2 = token;
	while (i < get_number_segment(arg) - 1)
	{
		tmp->str = NULL;
		tmp->type = 0;
		if (i != 0)
			tmp->prev = tmp2;
		tmp->next = malloc(sizeof(t_token));
		tmp2 = tmp;
		tmp = tmp->next;
		i++;
	}
	tmp->next = NULL;
}

void init(t_data *data, char *arg)
{
	data = malloc(sizeof(t_data));
	getcwd(data->pwd, PATH_MAX);
	data->token = malloc(sizeof(t_token));
	init_t_token(data->token, arg);
}
