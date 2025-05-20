/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 01:59:24 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/12 01:59:24 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <ctype.h>
# include "libft.h"
# include <stdarg.h>

# define PROMPT "minishell> "
# define MAX_PATH 4096

# define NO_QUOTE 0
# define SINGLE_QUOTE 1
# define DOUBLE_QUOTE 2

extern char		*g_heredoc_files[128];
extern int		g_heredoc_count;

typedef enum e_token_type
{
	T_WORD = 0,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	int				fd_in;
	int				fd_out;
	pid_t			pid;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_minishell
{
	char		**env;
	int			exit_status;
	t_cmd		*cmd;
	t_token		*tokens;
}	t_minishell;

typedef struct s_parser1
{
	t_minishell	*shell;
	t_token		*token;
	t_cmd		*cmd_list;
	t_cmd		**c;
	int			*redir_error;
}	t_parser1;

typedef struct s_process_redir_args
{
	t_minishell	*shell;
	t_cmd		*current;
	t_token		**token;
	t_cmd		*cmd_list;
	int			*redir_error;
}	t_process_redir_args;

typedef struct s_heredoc_data
{
	t_minishell	*shell;
	t_cmd		*current;
	t_token		**token;
	char		**heredoc_delims;
	int			*hdoc_count;
}	t_heredoc_data;

typedef struct s_redir_data
{
	t_minishell	*shell;
	t_cmd		*current;
	t_token		**token;
	t_cmd		*cmd_list;
	int			*redir_error;
}	t_redir_data;

typedef struct s_hpacc_data
{
	t_minishell	*shell;
	t_cmd		**cmd_list;
	t_cmd		**current;
	t_token		**token;
}	t_hpacc_data;

typedef struct s_process_args
{
	t_minishell	*shell;
	t_cmd		*current;
	t_token		**token;
	t_cmd		*cmd_list;
	int			*redir_error;
}	t_process_args;

typedef struct s_proc_red_data
{
	t_minishell		*shell;
	t_cmd			*current;
	t_token			**token;
	t_cmd			*cmd_list;
	int				*redir_error;
	t_token_type	redir_type;
}	t_proc_red_data;

typedef struct s_process_redirection_args
{
	t_minishell	*shell;
	t_cmd		*current;
	t_token		**token;
	t_cmd		*cmd_list;
	int			*redir_error;
}	t_process_redirection_args;

void	__write_heredoc_line(int fd, char *line,
	t_minishell *shell, int expand);
int	elsepid(int var[3], char *strs[3], t_minishell *shell, pid_t	pid);
void	ifpido(int var[3], char *strs[3], t_minishell *shell);
int	__should_expand_vars(char *original_delimiter);
char	*__process_delimiter(char *delimiter);
void	__p_d_c(int *isq, int *idq, int *var, char *delimiter);
char	*__get_heredoc_filename(void);
void	__handle_heredoc_signals(void);
void	__restore_signals(void);
char	*__make_env_var(t_minishell *shell, char *var, char *value);
int		__replace_env_var(t_minishell *s, char *var, char *n, size_t len);
void	init_shell(t_minishell *shell, char **env);
void	cleanup(t_minishell *shell);
void	shell_loop(t_minishell *shell);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
int		has_balanced_quotes(char *input);
int		length_within_quote(char quote_char, char *str, int pos);
void	handle_quotes(char *input, int *i, int *in_quote);
void	handle_redir_token(t_token **tokens, char *input, t_token_type type);
void	cpfd(const char *s, int fd);
t_token	*lexer(t_minishell *shell, char *input);
t_cmd	*parse(t_minishell *shell, t_token *token);
char	*process_filename(char *filename);
t_token	*create_token(char *value, t_token_type type);
void	token_add_back(t_token **tokens, t_token *new);
int		is_whitespace(char c);
int		is_metachar(char c);
void	expand_variables(t_minishell *shell, t_token *token);
char	*expand_word_with_quotes(t_minishell *shell, char *word);
char	*handle_dollar_sign(t_minishell *shell, char *str, int *i);
char	*handle_exit_status(t_minishell *shell, char *result);
char	*handle_env_var(t_minishell *shell, char *str, int *i, char *result);

void	process_cmd_args(t_cmd *cmd);
void	process_echo_args(t_cmd *cmd);
char	*strip_surrounding_quotes(const char *str);
void	execute(t_minishell *shell, t_cmd *cmd);
pid_t	launch_process(t_minishell *shell, t_cmd *cmd);
void	execute_builtin(t_minishell *shell, t_cmd *cmd);
int		is_builtin(char *cmd);
int		count_args(char **args);
int		execute_file(t_minishell *shell, char *file_path, char **args);

int		ft_echo(t_cmd *cmd);
int		ft_cd(t_minishell *shell, t_cmd *cmd);
int		ft_pwd(void);
int		ft_export(t_minishell *shell, t_cmd *cmd);
int		ft_unset(t_minishell *shell, t_cmd *cmd);
int		ft_env(t_minishell *shell);
int		ft_exit(t_minishell *shell, t_cmd *cmd);

char	**copy_env(char **env);
char	*get_env_var(t_minishell *shell, char *var);
int		set_env_var(t_minishell *shell, char *var, char *value);
int		unset_env_var(t_minishell *shell, char *var);
void	print_env(t_minishell *shell);
int		is_valid_env_name(char *name);

void	free_array(char **arr);
void	free_env(char **env);
void	free_tokens(t_token *tokens);
void	free_cmds(t_cmd *cmd);
char	*join_strs(char *s1, char *s2);
void	print_error(char *cmd, char *arg, char *error);
char	*ft_strndup(const char *s, size_t n);

/* Libft-like utils */
size_t	ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
long	ft_atol(const char *str);
int		ft_sprintf(char *str, const char *fmt, ...);

//parse
void	__check_quotes(char c, int *in_single_quote, int *in_double_quote);
int		__is_entirely_in_single_quotes(char *str);
void	expand_variables(t_minishell *shell, t_token *token);
char	*__append_char(char *result, char c);
char	*__handle_quotes(char *result, char c, int *quote_flag);
char	*__handle_dollar(t_minishell *shell, char *result, char *word, int *i);
char	*expand_word_with_quotes(t_minishell *shell, char *word);
char	*handle_dollar_sign(t_minishell *shell, char *str, int *i);
char	*handle_exit_status(t_minishell *shell, char *result);
char	*__append_dollar(char *result);
char	*__append_env_value(char *result, char *value);
void	__extract_var_name(char *str, int *i, char *var_name);
char	*handle_env_var(t_minishell *shell, char *str, int *i, char *result);
void	__process_quotes(char c, int *in_single_quote, int *in_double_quote);
void	__copy_char(char *processed, char c, int *write_pos);
char	*process_concat_filename(char *filename);
t_cmd	*__create_new_cmd(t_cmd *cmd_list, t_cmd **current);
char	**__add_argument(char **args, char *value, t_cmd *cmd_list);
int		__handle_redirection(t_minishell *shell, t_cmd *curren,
			t_token_type redir_type, char *fl);
int		__process_redirection(t_process_redir_args args);
t_cmd	*parse(t_minishell *shell, t_token *token);
int		__is_quote(char c, int in_single_quote, int in_double_quote);
int		__process_concat_loop(char *filename, char *processed, int len);
t_cmd	*__hpacc(t_minishell *shell, t_cmd **cmd_list,
			t_cmd **current, t_token **token);
int		__handle_redir_error(t_minishell *shell, char *filename,
			int *redir_error);
int		__redir_syntax_error(t_minishell *shell, t_token_type redir_type,
			t_cmd *cmd_list);
int		__handle_pipe_or_init(t_minishell *shell, t_cmd **cmd_list,
			t_cmd **current, t_token **token);
void	__setup_parsing(t_cmd **cmd_list, t_cmd **current, int *redir_error);
int		__check_first_pipe(t_minishell *shell, t_token *token);
int		__handle_heredoc_redir(t_proc_red_data *d, t_redir_data *data);
int		__process_other_redir(t_redir_data *data, t_token_type redir_type);
void	__process_token(t_process_args *args);

//main
void	cleanup_parse_error(t_minishell *shell);
void	__handle_redirection_main(t_token **tokens, char *input, int *i);
void	__handle_quotes_main(char *input, int *i, int *in_quote);
void	__pt_main(t_token **t, char *input, int *i, int *in_quote);
t_token	*lexer(t_minishell *shell, char *input);
char	*__process_quotes_main2(char *filename, int *read_pos,
			int *in_single, int *in_double);
char	*get_redir_value(char *input, t_token_type type);
int		__is_whitespace_no_quote(char c, int in_quote);
int		__is_pipe_no_quote(char c, int in_quote);
void	__handle_quote_state(char c, int *in_single, int *in_double);
void	__free_and_replace_input(char **input, char *remaining_cmd);

//exectution
void	__redir_fd(int fd, int std);
void	__close_other_fds(t_cmd *cmd);
int		__exec_path_cmd(t_minishell *shell, t_cmd *cmd);
void	__setup_pipes(t_cmd *cmd);
void	__wait_pipeline(t_minishell *shell, t_cmd *cmd,
			pid_t last_pid, int *last_was_empty);
void	__exec_single_builtin(t_minishell *shell, t_cmd *cmd);
void	__close_fds(t_minishell *shell, t_cmd *cmd);

//builtins
int		ft_cmp_env(const void *a, const void *b);
int		__setup_heredoc(t_minishell *shell, char *delimiter);
int		__setup_heredocs(t_minishell *shell, char **delimiters);

#endif
