/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samberna <samberna@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:21:00 by tcarlier          #+#    #+#             */
/*   Updated: 2025/04/28 01:54:51 by samberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void init_t_token(t_token *token, char *arg)
{
	t_token	*tmp;
	t_token	*tmp2;
	int i;
	
	i = 0;
	token->str = NULL;
	token->type = 0;
	token->prev = NULL;
	token->next = NULL;
	tmp = malloc(sizeof(t_token));
	token->next = tmp;
	tmp->prev = token;
	tmp2 = token;
	while (i < get_number_segment(arg) - 1)
	{
		tmp->str = NULL;
		tmp->type = -1;
		tmp->next = NULL;
		if (i != 0)
			tmp->prev = tmp2;
		if (i != get_number_segment(arg) - 1)
			tmp->next = malloc(sizeof(t_token));
		tmp2 = tmp;
		tmp = tmp->next;
		i++;
	}
	tmp->next = NULL;
}

void init(t_data **data, char *arg)
{
    *data = malloc(sizeof(t_data));
	(*data)->pwd = malloc(PATH_MAX);
    if (!(*data)->pwd)
        return;
    getcwd((*data)->pwd, PATH_MAX);
    (*data)->token = malloc(sizeof(t_token));
    if (!(*data)->token)
    {
        free((*data)->pwd);
        return;
    }
    
    init_t_token((*data)->token, arg);
}

// <<
// cat arg << eof -> cat arg -> pipe[0] = NULL
// cat << eof -> cat << -> pipe[0] = NULL   les heredoc sont replacer avant d'executer la function, chaque heredoc sont appeller avant le flow de l'exec (de la droite vert la gauche)
// cat arg1 << eof arg2 -> cat arg1 ; cat arg2

// < 
// pareil que  << mais pour les nom de fichiers -> cat < fichier != cat < texte
// Donc pipe[0] = fd du nom du fichier si pas d'arg avant
// cat < fichier arg -> cat fichier ; cat arg

// >
// cat arg > fichier -> ecrit dans le fichier -> pipe[0] = NULL pipe[1] = fd du fichier
// >fichier < arg -> juste creer fichier

// >>
// Pareil que  > mais ecrit en append a la fin d'un fichier -> append != ecraser

// |
// cat fichier | wc -l -> print nb de ligne -> (cat) pipe[0] = fd_fichier pipe[1] = stdin de wc; (wc) pipe[0] = stdin pipe[1] = sortie standard
// cas special : cmd arg | echo -> print '\n' -> (cmd) pipe[0] = arg pipe[1] = stdin de echo; (echo) pas de stdin juste un arg obligé

