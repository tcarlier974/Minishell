/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:44:55 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/20 14:50:49 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	__redir_fd(int fd, int std)
{
	if (fd != std)
	{
		if (fd < 0)
		{
			if (std == STDIN_FILENO)
				close(STDIN_FILENO);
			exit(1);
		}
		dup2(fd, std);
		close(fd);
	}
}
