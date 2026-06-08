/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by fcardozo          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

int	exp_listgrow(t_arglist *list)
{
	char	**next;
	int		i;

	if (list->size + 1 < list->cap)
		return (0);
	if (list->cap == 0)
		list->cap = 8;
	else
		list->cap *= 2;
	next = malloc(sizeof(char *) * list->cap);
	if (!next)
		return (1);
	i = 0;
	while (i < list->size)
	{
		next[i] = list->items[i];
		i++;
	}
	return (free(list->items), list->items = next, 0);
}

int	exp_listadd(t_arglist *list, char *item)
{
	if (exp_listgrow(list))
		return (free(item), 1);
	list->items[list->size++] = item;
	list->items[list->size] = NULL;
	return (0);
}

int	exp_flush_word(t_arglist *list, t_word *word)
{
	char	*out;

	if (!word->active)
		return (0);
	out = ft_strdup(word->buf);
	if (!out)
		return (1);
	if (exp_listadd(list, out))
		return (1);
	word->len = 0;
	word->active = 0;
	if (word->buf)
		word->buf[0] = '\0';
	return (0);
}

int	exp_add_unquoted(t_arglist *list, t_word *word, const char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if (sh_isspace(value[i]) && exp_flush_word(list, word))
			return (1);
		while (value[i] && sh_isspace(value[i]))
			i++;
		if (value[i] && !sh_isspace(value[i])
			&& exp_wordchar(word, value[i++]))
			return (1);
	}
	return (0);
}
