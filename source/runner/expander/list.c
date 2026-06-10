/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:01 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:01 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

int	exp_flush_word(t_array *list, t_word *word)
{
	char	*out;

	if (!word->active)
		return (0);
	if (word->buf)
		out = ft_strdup(word->buf);
	else
		out = ft_strdup("");
	if (!out)
		return (1);
	if (!ft_array_append(list, &out))
	{
		free(out);
		return (1);
	}
	word->len = 0;
	word->active = 0;
	if (word->buf)
		word->buf[0] = '\0';
	return (0);
}

int	exp_add_unquoted(t_array *list, t_word *word, const char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if (sh_isspace(value[i]) && exp_flush_word(list, word))
			return (1);
		while (value[i] && sh_isspace(value[i]))
			i++;
		if (value[i] && !sh_isspace(value[i]) && exp_wordchar(word, value[i++]))
			return (1);
	}
	return (0);
}
