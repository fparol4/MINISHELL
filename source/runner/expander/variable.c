/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:57 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:57 by fcardozo         ###   ########.fr       */
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

int	exp_var(t_exp_ctx *ctx)
{
	char	*value;

	value = exp_varvalue(ctx->arg, ctx->i, ctx->env);
	if (!value)
		return (exp_wordchar(ctx->word, '$'));
	return (exp_add_unquoted(ctx->list, ctx->word, value));
}

int	exp_var_quoted(t_exp_ctx *ctx)
{
	char	*value;

	value = exp_varvalue(ctx->arg, ctx->i, ctx->env);
	if (!value)
		return (exp_wordchar(ctx->word, '$'));
	return (exp_wordstr(ctx->word, value));
}
