/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:05:43 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 23:16:50 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	clean_token(t_token *token)
{
	t_token	*tmp;
	
	tmp = token;
	while (tmp->next)
	{
		token = token->next;
		free(tmp);
		tmp = token;
	}
}

void	clean(t_data *data)
{
	clean_token(data->token);
	free(data->pwd);
	free(data);
}