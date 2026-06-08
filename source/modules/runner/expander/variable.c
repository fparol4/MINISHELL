/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by fcardozo          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

static char	*var_special(int *i, t_env **env)
{
	char	*value;

	(*i)++;
	value = env_get(env, ENV_ERRCODE);
	if (!value)
		return ("");
	return (value);
}

char	*exp_varvalue(char *arg, int *i, t_env **env)
{
	char	*key;
	char	*value;
	int		start;

	(*i)++;
	if (arg[*i] == '?')
		return (var_special(i, env));
	if (!sh_varstart(arg[*i]))
		return (NULL);
	start = *i;
	while (sh_varchar(arg[*i]))
		(*i)++;
	key = ft_substr(arg, start, *i - start);
	if (!key)
		return ("");
	value = env_get(env, key);
	free(key);
	if (!value)
		return ("");
	return (value);
}

int	exp_var(t_arglist *list, t_word *word, char *arg, int *i, t_env **env)
{
	char	*value;

	value = exp_varvalue(arg, i, env);
	if (!value)
		return (exp_wordchar(word, '$'));
	return (exp_add_unquoted(list, word, value));
}

int	exp_var_quoted(t_word *word, char *arg, int *i, t_env **env)
{
	char	*value;

	value = exp_varvalue(arg, i, env);
	if (!value)
		return (exp_wordchar(word, '$'));
	return (exp_wordstr(word, value));
}
