/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilieer <tcarlilee@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:19:41 by tcarlier          #+#    #+#             */
/*   Updated: 2025/03/13 16:19:51 by tcarlilee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(void)
{
	char	*pwd;

	pwd = malloc(PATH_MAX);
	if (!pwd)
	{
		perror("minishell");
		return ;
	}
	if (!getcwd(pwd, PATH_MAX))
	{
		perror("minishell");
		free(pwd);
		return ;
	}
	printf("%s\n", pwd);
	free(pwd);
}
