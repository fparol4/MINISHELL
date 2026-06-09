/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:46 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:46 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

static int	rn_argexpand(char *arg, t_array *out, t_env **env)
{
	t_exp_ctx	ctx;
	t_word		word;
	int			i;

	ft_bzero(&word, sizeof(t_word));
	ctx.list = out;
	ctx.word = &word;
	ctx.arg = arg;
	ctx.i = &i;
	ctx.env = env;
	i = 0;
	while (arg && arg[i])
	{
		if (exp_process_char(&ctx))
		{
			free(word.buf);
			return (1);
		}
	}
	if (exp_flush_word(out, &word))
	{
		free(word.buf);
		return (1);
	}
	free(word.buf);
	return (0);
}

char	**rn_expand(char **args, t_env **env)
{
	t_array	out;
	char	*null_term;
	int		i;

	if (ft_array_init(&out, sizeof(char *)))
		return (NULL);
	i = 0;
	while (args && args[i])
	{
		if (rn_argexpand(args[i], &out, env))
		{
			sh_freeargs((char **)out.items);
			return (NULL);
		}
		i++;
	}
	null_term = NULL;
	if (!ft_array_append(&out, &null_term))
	{
		sh_freeargs((char **)out.items);
		return (NULL);
	}
	return ((char **)out.items);
}
