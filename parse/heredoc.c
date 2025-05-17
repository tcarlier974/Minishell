/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:55:12 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/14 22:24:25 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *g_heredoc_files[128];
static int  g_heredoc_count = 0;

static void __store_heredoc_filename(char *filename)
{
    if (g_heredoc_count < 128)
        g_heredoc_files[g_heredoc_count++] = ft_strdup(filename);
}

/*
Pseudo-code commenté décrivant l’approche :

1. Initialiser une structure (ex: liste_heredoc) pour stocker (delim, contenu).
2. Parcourir la ligne de commande pour trouver chaque "<< DELIM" et remplir liste_heredoc.
3. Pour chaque élément (delim, contenu) :
   - Tant que l’utilisateur n’a pas saisi delim :
     - Afficher "heredoc> "
     - Lire la ligne
     - Comparer avec delim
     - Si différent, ajouter la ligne à contenu
     - Sinon, quitter cette boucle
4. Après avoir rempli tous les buffers, les écrire sur la sortie standard.
5. Libérer chaque tampon pour éviter les fuites.
*/

static char	*__get_heredoc_filename(void)
{
    static int	counter = 0;
    char		filename[32];

    snprintf(filename, sizeof(filename), "/tmp/heredoc_%d", counter++);
    __store_heredoc_filename(filename);
    return (ft_strdup(filename));
}

static void	__handle_heredoc_signals(void)
{
	signal(SIGINT, SIG_DFL);  /* Reset to default behavior for CTRL+C */
	signal(SIGQUIT, SIG_IGN); /* Ignore CTRL+\ */
}

static void	__restore_signals(void)
{
	signal(SIGINT, handle_sigint);   /* Restore our custom handler */
	signal(SIGQUIT, handle_sigquit); /* Restore our custom handler */
}

/* Process delimiter to remove quotes if present */
static char *__process_delimiter(char *delimiter)
{
	char *processed;
	int len;
	int in_single_quote = 0;
	int in_double_quote = 0;
	int i = 0;
	int j = 0;

	len = ft_strlen(delimiter);
	processed = malloc(len + 1);
	if (!processed)
		return (NULL);

	while (delimiter[i])
	{
		if ((delimiter[i] == '\'' && !in_double_quote) || 
			(delimiter[i] == '"' && !in_single_quote))
		{
			if (delimiter[i] == '\'' && !in_single_quote)
				in_single_quote = 1;
			else if (delimiter[i] == '\'' && in_single_quote)
				in_single_quote = 0;
			else if (delimiter[i] == '"' && !in_double_quote)
				in_double_quote = 1;
			else if (delimiter[i] == '"' && in_double_quote)
				in_double_quote = 0;
			i++;
		}
		else
			processed[j++] = delimiter[i++];
	}
	processed[j] = '\0';
	return (processed);
}

/* Determines if we should expand variables based on delimiter quoting */
static int __should_expand_vars(char *original_delimiter)
{
	int i = 0;

	/* If delimiter is quoted with single quotes, don't expand variables */
	while (original_delimiter[i])
	{
		if (original_delimiter[i] == '\'')
			return (0);
		i++;
	}
	return (1);
}

/* Write heredoc line with possible expansion */
static void __write_heredoc_line(int fd, char *line, t_minishell *shell, int expand)
{
	char *expanded;

	if (expand && ft_strchr(line, '$'))
	{
		expanded = expand_word_with_quotes(shell, line);
		write(fd, expanded, ft_strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

int	__setup_heredoc(t_minishell *shell, char *delimiter)
{
	char	*line;
	char	*heredoc_file;
	char    *processed_delimiter;
	int		fd;
	pid_t	pid;
	int		status;
	int     expand_vars;

	heredoc_file = __get_heredoc_filename();
	if (!heredoc_file)
		return (-1);

	/* Save original delimiter to check quoting */
	expand_vars = __should_expand_vars(delimiter);
	
	/* Process delimiter to remove quotes */
	processed_delimiter = __process_delimiter(delimiter);
	if (!processed_delimiter)
	{
		free(heredoc_file);
		return (-1);
	}

	/* Use fork to handle signals properly in heredoc mode */
	pid = fork();
	if (pid == -1)
	{
		free(heredoc_file);
		free(processed_delimiter);
		return (-1);
	}

	if (pid == 0)
	{
		/* Child process */
		__handle_heredoc_signals();
		fd = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd < 0)
			exit(1);
		while (1)
		{
			line = readline("heredoc> ");
			if (!line || !ft_strcmp(line, processed_delimiter))
			{
				free(line);
				break ;
			}
			__write_heredoc_line(fd, line, shell, expand_vars);
			free(line);
		}
		close(fd);
		free(processed_delimiter);
		free(heredoc_file);
		exit(0);
	}
	else
	{
		/* Parent process */
		waitpid(pid, &status, 0);
		__restore_signals();
		free(processed_delimiter);
		
		/* Check if heredoc was interrupted by SIGINT */
		if (WIFSIGNALED(status))
		{
			shell->exit_status = 128 + WTERMSIG(status);
			unlink(heredoc_file);
			free(heredoc_file);
			return (-1);
		}

		fd = open(heredoc_file, O_RDONLY);
		unlink(heredoc_file); /* Delete the temporary file */
		free(heredoc_file);
		return (fd);
	}
}

int	__setup_heredocs(t_minishell *shell, char **delimiters)
{
    char	*heredoc_file = __get_heredoc_filename();
    int		fd;
    pid_t	pid;
    int		status;

    if (!heredoc_file)
        return (-1);
    pid = fork();
    if (pid == 0)
    {
        __handle_heredoc_signals();
        fd = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
        if (fd < 0)
            exit(1);
        
        /* Process each delimiter in order */
        int i = 0;
        while (delimiters[i])
        {
            char	*line;
            char    *processed_delimiter;
            int     expand_vars;

            /* Save original delimiter to check quoting */
            expand_vars = __should_expand_vars(delimiters[i]);
			
			/* Process delimiter to remove quotes */
			processed_delimiter = __process_delimiter(delimiters[i]);
			if (!processed_delimiter)
				exit(1);
                
            /* Read lines until delimiter is found */
            while (1)
			{
				line = readline("heredoc> ");
				if (!line || !ft_strcmp(line, processed_delimiter))
				{
					free(line);
					break;
				}
				__write_heredoc_line(fd, line, shell, expand_vars);
				free(line);
			}
			free(processed_delimiter);
			i++;
        }
        close(fd);
        exit(0);
    }
    else
    {
        waitpid(pid, &status, 0);
        __restore_signals();
        if (WIFSIGNALED(status))
        {
            shell->exit_status = 128 + WTERMSIG(status);
            unlink(heredoc_file);
            free(heredoc_file);
            return (-1);
        }
        fd = open(heredoc_file, O_RDONLY);
        unlink(heredoc_file);
        free(heredoc_file);
        return (fd);
    }
}
