/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:19:17 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:19:17 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_minishell *shell, char **env)
{
	shell->env = copy_env(env);
	shell->exit_status = 0;
	shell->cmd = NULL;
	shell->tokens = NULL;
}

void	cleanup(t_minishell *shell)
{
	if (shell->env)
		free_env(shell->env);
	if (shell->cmd)
		free_cmds(shell->cmd);
	if (shell->tokens)
		free_tokens(shell->tokens);
}

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
}

int	has_balanced_quotes(char *input)
{
	int	in_single;
	int	in_double;
	int	i;

	in_single = 0;
	in_double = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (input[i] == '"' && !in_single)
			in_double = !in_double;
		i++;
	}
	if (in_single || in_double)
		return (0);
	return (1);
}
