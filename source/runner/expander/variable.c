/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:15 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:15 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

static char	*exp_var_status(int *i, t_env **env)
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
	{
		value = exp_var_status(i, env);
		return (value);
	}
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
	int		status;

	value = exp_varvalue(ctx->arg, ctx->i, ctx->env);
	if (!value)
	{
		status = exp_wordchar(ctx->word, '$');
		return (status);
	}
	status = exp_add_unquoted(ctx->list, ctx->word, value);
	return (status);
}

int	exp_var_quoted(t_exp_ctx *ctx)
{
	char	*value;
	int		status;

	value = exp_varvalue(ctx->arg, ctx->i, ctx->env);
	if (!value)
	{
		status = exp_wordchar(ctx->word, '$');
		return (status);
	}
	status = exp_wordstr(ctx->word, value);
	return (status);
}
