/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 09:56:33 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/10 00:10:09 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// token
/*
# define INPUT		1	//"<"
# define HEREDOC	2	//"<<"
# define TRUNC		3	//">"
# define APPEND		4	//">>"
# define PIPE		5	//"|"
# define CMD		6	//"|"
# define ARG		7	//"|"
*/
enum nigger_enum_e {
	INPUT,
	HEREDOC,
	TRUNC,
	APPEND,
	PIPE,
	CMD,
	ARG,
};

/*
	[INPUT] = < fd in (sdtdin) = stdout of file (example: wc -l < /etc/passwd)
	[HEREDOC] = << https://phoenixnap.com/kb/bash-heredoc
	[TRUNC]/[APPEND] =  tristan@MacBook-Pro-de-Tristan Minishell % echo 'test2' > a.txt   
tristan@MacBook-Pro-de-Tristan Minishell % cat a.txt           
test2
tristan@MacBook-Pro-de-Tristan Minishell % echo 'test2' >> a.txt  
tristan@MacBook-Pro-de-Tristan Minishell % echo 'test2' >> a.txt
tristan@MacBook-Pro-de-Tristan Minishell % cat a.txt            
test2
test2
test2
tristan@MacBook-Pro-de-Tristan Minishell %



static void	copy_token(char *command, int length, char *str, int i)
{
	int	j;

	j = 0;
	while (command[i + j] && i < length)
	{
		if (command[i + j] == '\'' && ++j)
		{
			while (command[i + j] != '\'' && ++i)
				str[i - 1] = command[(i - 1) + j];
			j++;
		}
		else if (command[i + j] == '"' && ++j)
		{
			while (command[i + j] != '"' && ++i)
				str[i - 1] = command[(i - 1) + j];
			j++;
		}
		else
		{
			str[i] = command[i + j];
			i++;
		}
	}
	str[i] = 0;
}
	
*/

int	is_ratiel(char *str)
{
	if (str && *str && ft_strlen(str) >= 2)
	{
		if (!ft_strncmp(str, "<<", 2))
			return (HEREDOC);
		if (!ft_strncmp(str, ">>", 2))
			return (APPEND);
	}
	if (*str && ft_strlen(str) >= 1)
	{
		if (!ft_strncmp(str, "<", 1))
			return (INPUT);
		if (!ft_strncmp(str, ">", 1))
			return (TRUNC);
		if (!ft_strncmp(str, "|", 1))
			return (PIPE);
	}
	return (0);
}

void open_quote(char *args)
{
	int	i;
	int	c1;
	int	c2;

	i = 0;
	c1 = 0;
	c2 = 0;
	while(args[i])
	{
		if (args[i] == '\'')
			c1++;
		else if (args[i] == '\"')
			c2++;
		i++;
	}
	if (c1 % 2 != 0 || c2 % 2 != 0)
		//print_error("open quote", 1);
		;
}

int length_until_spe(char *l, int i)
{
	size_t z = 0;
	
	while (l[i + z] && !is_ratiel(&l[i + z]))
		z++;
	if (ft_strlen(l) == i + z)
		return -1;
	return z;
}

// /bin/ls 'hellozzad zadad' "$(TEST_ENV) test" yes yes = 5
// /bin/ls 'THIS IS A LONG ASS TEST PHRASE' = 2
int get_number_segment(char *str)
{
	int	i;
	int	s;

	i = 0;
	s = 0;
	while(str[i] == ' ')
	{
		i++;
	}
	while (str[i])
	{
		if (str[i] == ' ')
		{
			while (str[i] == ' ')
				i++;
		}
		else if (str[i] == '\"')
		{
			i += 2;
			while (str[i-1] != '\"')
				i++;
		}
		else if (str[i] == '\'')
		{
			i += 2;
			while (str[i-1] != '\'')
				i++;
		}
		s++;
		i++;
	}
	return (s);
}

void extract_cmd(char *str)
{
	int		s;
	int		e;
	int		k;
	int		q;

	k = 0;
	s = 0;
	q = 0;
	while (k < get_number_segment(str) && str[s])
	{
		while (str[s] == ' ' && str[s])
			s++;
		e = s;
		if (str[s] != '\'' && str[s] != '\"')
		{
			while (str[e] != ' ' && str[e])
				e++;
		}
		else if (str[s] == '\'')
		{
			q = 1;
			while ((str[e] != ' ' || q == 1) && str[e])
			{
				e++;
				if (str[e] == '\'')
					q = 0;
			}
		}
		else if (str[s] == '\"')
		{
			q = 1;
			while ((str[e] != ' ' || q == 1) && str[e])
			{
				e++;
				if (str[e] == '\"')
					q = 0;
			}
		}
		printf("extracted %s\n", ft_strndup((char *)str + s, e - s));
		k++;
		s = e;
	}
}

t_token *parse(char *line, char **envp)
{
	t_token *head = NULL;
	int i = 0;
	(void)envp;
	while (line[i])
	{
		//while (is_space(line[i]))
		//	i++;
		if (line[i] && !is_ratiel(&line[i]))
		{
			int lus = length_until_spe(line, i);
			if (lus != -1)
			{
				printf("%d, %s\n", lus, strndup(&line[i], lus));
				line += lus;
				continue;
			}
			printf("class: %c %d\n", line[i], length_until_spe(line, i));
			i++;
			continue;
		}
		printf("spe: %c %d\n", line[i], length_until_spe(line, i));
		i++;
	}
	
	return head;
}