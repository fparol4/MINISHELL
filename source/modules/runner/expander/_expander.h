/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _expander.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by fcardozo          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EXPANDER_H
# define _EXPANDER_H

# include "../../../../headers/runner.h"

typedef struct s_arglist
{
	char	**items;
	int		size;
	int		cap;
}	t_arglist;

typedef struct s_word
{
	char	*buf;
	int		len;
	int		cap;
	int		active;
}	t_word;

int		exp_listgrow(t_arglist *list);
int		exp_listadd(t_arglist *list, char *item);
int		exp_flush_word(t_arglist *list, t_word *word);
int		exp_add_unquoted(t_arglist *list, t_word *word, const char *value);
int		exp_wordchar(t_word *word, char c);
int		exp_wordstr(t_word *word, const char *str);
int		exp_var(t_arglist *list, t_word *word, char *arg, int *i,
			t_env **env);
int		exp_var_quoted(t_word *word, char *arg, int *i, t_env **env);
int		exp_single(t_word *word, char *arg, int *i);
int		exp_double(t_word *word, char *arg, int *i, t_env **env);
int		exp_arg_char(t_arglist *out, t_word *word, char *arg, int *i,
			t_env **env);
char	*exp_varvalue(char *arg, int *i, t_env **env);

#endif
