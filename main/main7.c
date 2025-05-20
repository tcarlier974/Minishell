/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main7.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:20:01 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:20:01 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__handle_empty_var(t_minishell *shell, char **input, char *space)
{
	char	*remaining_cmd;

	(void)shell;
	remaining_cmd = ft_strdup(space + 1);
	if (!remaining_cmd)
		return ;
	while (*remaining_cmd && *remaining_cmd == ' ')
		remaining_cmd++;
	if (*remaining_cmd)
		__free_and_replace_input(input, remaining_cmd);
	free(remaining_cmd);
}

void	__handle_env_var(t_minishell *shell, char **input)
{
	char	*space;
	char	*var_name;
	char	*var_value;

	space = ft_strchr(*input, ' ');
	if (!space)
		return ;
	var_name = ft_strndup(*input + 1, space - *input - 1);
	if (!var_name)
		return ;
	var_value = get_env_var(shell, var_name);
	free(var_name);
	if (!var_value || !*var_value)
		__handle_empty_var(shell, input, space);
}

static void	pouleto(t_cmd	*current)
{
	while (current)
	{
		if (current->args && current->args[0])
			process_cmd_args(current);
		current = current->next;
	}
}

void	__process_input(t_minishell *shell, char *input)
{
	t_token	*tokens;
	t_cmd	*current;

	add_history(input);
	if (input[0] == '$')
		__handle_env_var(shell, &input);
	tokens = lexer(shell, input);
	if (!tokens)
		return ;
	expand_variables(shell, tokens);
	shell->cmd = parse(shell, tokens);
	shell->tokens = tokens;
	if (!shell->cmd)
		return (cleanup_parse_error(shell));
	current = shell->cmd;
	pouleto(current);
	execute(shell, shell->cmd);
	free_cmds(shell->cmd);
	shell->cmd = NULL;
	if (shell->tokens)
		free_tokens(shell->tokens);
	shell->tokens = NULL;
}

void	shell_loop(t_minishell *shell)
{
	char		*input;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			__process_input(shell, input);
		free(input);
	}
}
