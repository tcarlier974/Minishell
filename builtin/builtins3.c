/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:57:35 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:57:35 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_minishell *shell)
{
	print_env(shell);
	return (0);
}

static int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	__exit_parse_arg(char *arg, long *code)
{
	int		sign;
	char	*processed;
	char	*clean_arg;

	sign = 1;
	processed = process_filename(arg);
	clean_arg = strip_surrounding_quotes(processed);
	free(processed);
	if (clean_arg[0] == '+')
		clean_arg++;
	else if (clean_arg[0] == '-')
	{
		sign = -1;
		clean_arg++;
	}
	if (!ft_is_numeric(clean_arg))
	{
		print_error("exit", arg, "numeric argument required");
		exit(2);
	}
	*code = ft_atol(clean_arg) * sign;
	return (0);
}

int	ft_exit(t_minishell *shell, t_cmd *cmd)
{
	long	code;
	int		exit_code;

	exit_code = shell->exit_status;
	if (cmd->args[1])
	{
		if (cmd->args[2])
			return (print_error("exit", NULL, "too many arguments"), 1);
		__exit_parse_arg(cmd->args[1], &code);
		exit_code = code % 256;
		if (exit_code < 0)
			exit_code += 256;
	}
	cleanup(shell);
	write(1, "exit\n", 5);
	exit(exit_code);
}
