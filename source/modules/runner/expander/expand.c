/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by fcardozo          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

static int	rn_argexpand(char *arg, t_arglist *out, t_env **env)
{
	t_word	word;
	int		i;

	ft_bzero(&word, sizeof(t_word));
	i = 0;
	while (arg && arg[i])
	{
		if (exp_arg_char(out, &word, arg, &i, env))
			return (free(word.buf), 1);
	}
	if (exp_flush_word(out, &word))
		return (free(word.buf), 1);
	free(word.buf);
	return (0);
}

char	**rn_expand(char **args, t_env **env)
{
	t_arglist	out;
	int			i;

	ft_bzero(&out, sizeof(t_arglist));
	i = 0;
	while (args && args[i])
	{
		if (rn_argexpand(args[i], &out, env))
		{
			sh_freeargs(out.items);
			return (NULL);
		}
		i++;
	}
	if (exp_listgrow(&out))
		return (sh_freeargs(out.items), NULL);
	out.items[out.size] = NULL;
	return (out.items);
}
