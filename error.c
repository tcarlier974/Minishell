/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 20:23:39 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 02:31:48 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	print_error(t_data *data, char *str, int code)
{
	(void)data;
	//clean(data);
	write(2, &str, ft_strlen(str));
	exit(code);
}