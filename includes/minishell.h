/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 00:13:55 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/26 02:2:177:46 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <stdbool.h>
# include <dirent.h>
# include <sys/stat.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT "@minishell$ "

typedef enum e_token_type {
	INPUT,
	HEREDOC,
	TRUNC,
	APPEND,
	PIPE,
	CMD,
	ARG,
} t_token_type;

typedef struct s_token
{
	char			*str; // raw parsed (splited on ' ')
	t_token_type	type; // token enum
	int				pipe[2]; //stdou qnd stdin // default 1 and 2 (fd) stderr = 3 (never change)
	struct s_token	*prev; // null = first
	struct s_token	*next; // null = last
}				t_token;

typedef struct s_data
{
	t_token	*token;
	char	*pwd;
	
}				t_data;

void	init(t_data *data, char *arg);
t_token	*tab_to_lst(char *line, t_data *data);
char	**ft_split(char const *s1, char c);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_exec(char **args, char **envp);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s);
int 	ft_strstrlen(char **t);
t_token *parse(char *args, char **envp);
void	print_error(t_data *data, char *str, int code);
void	open_quote(char *args);
int		is_space(char c);
int 	get_number_segment(char *str);
char 	**extract_cmd(char *str);
char	*ft_strndup(char *s1, int len);
t_token_type	is_ratiel(char *str);
#endif