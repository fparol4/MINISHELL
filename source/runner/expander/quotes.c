/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:57 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:57 by fcardozo         ###   ########.fr       */
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

int	exp_double(t_exp_ctx *ctx)
{
	(*ctx->i)++;
	ctx->word->active = 1;
	while (ctx->arg[*ctx->i] && ctx->arg[*ctx->i] != '"')
	{
		if (ctx->arg[*ctx->i] == '$')
		{
			if (exp_var_quoted(ctx))
				return (1);
		}
		else if (exp_wordchar(ctx->word, ctx->arg[(*ctx->i)++]))
			return (1);
	}
	if (ctx->arg[*ctx->i] == '"')
		(*ctx->i)++;
	return (0);
}

int	exp_arg_char(t_exp_ctx *ctx)
{
	if (ctx->arg[*ctx->i] == '\'')
		return (exp_single(ctx->word, ctx->arg, ctx->i));
	if (ctx->arg[*ctx->i] == '"')
		return (exp_double(ctx));
	if (ctx->arg[*ctx->i] == '$')
		return (exp_var(ctx));
	if (sh_isspace(ctx->arg[*ctx->i]))
	{
		if (exp_flush_word(ctx->list, ctx->word))
			return (1);
		while (sh_isspace(ctx->arg[*ctx->i]))
			(*ctx->i)++;
		return (0);
	}
	return (exp_wordchar(ctx->word, ctx->arg[(*ctx->i)++]));
}
