/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by fcardozo          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

int	exp_single(t_word *word, char *arg, int *i)
{
	(*i)++;
	word->active = 1;
	while (arg[*i] && arg[*i] != '\'')
	{
		if (exp_wordchar(word, arg[*i]))
			return (1);
		(*i)++;
	}
	if (arg[*i] == '\'')
		(*i)++;
	return (0);
}

int	exp_double(t_word *word, char *arg, int *i, t_env **env)
{
	(*i)++;
	word->active = 1;
	while (arg[*i] && arg[*i] != '"')
	{
		if (arg[*i] == '$')
		{
			if (exp_var_quoted(word, arg, i, env))
				return (1);
		}
		else if (exp_wordchar(word, arg[(*i)++]))
			return (1);
	}
	if (arg[*i] == '"')
		(*i)++;
	return (0);
}

int	exp_arg_char(t_arglist *out, t_word *word, char *arg, int *i,
	t_env **env)
{
	if (arg[*i] == '\'')
		return (exp_single(word, arg, i));
	if (arg[*i] == '"')
		return (exp_double(word, arg, i, env));
	if (arg[*i] == '$')
		return (exp_var(out, word, arg, i, env));
	if (sh_isspace(arg[*i]))
	{
		if (exp_flush_word(out, word))
			return (1);
		while (sh_isspace(arg[*i]))
			(*i)++;
		return (0);
	}
	return (exp_wordchar(word, arg[(*i)++]));
}
