/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 19:24:38 by samberna          #+#    #+#             */
/*   Updated: 2025/05/10 19:24:38 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void compress_token(t_data *data)
{
	t_token *head = NULL;
	t_token *current = NULL;

	head = data->token;
	while (head != NULL)
	{
		if (head->type == CMD)
		{
			current = head;
			while (current->next && current->next->type == ARG)
			{
				current->str = ft_strjoin(current->str, ft_strjoin(" ", current->next->str));
				t_token *temp = current->next;
				current->next = current->next->next;
				free(temp->str);
				free(temp);
			}
		}
		head = head->next;
	}
}
