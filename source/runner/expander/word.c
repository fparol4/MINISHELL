/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:57 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:57 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_expander.h"

static int	wordgrow(t_word *word, int add)
{
	char	*next;
	int		cap;

	if (word->len + add + 1 <= word->cap)
		return (0);
	cap = word->cap;
	if (cap == 0)
		cap = 32;
	while (word->len + add + 1 > cap)
		cap *= 2;
	next = malloc(sizeof(char) * cap);
	if (!next)
		return (1);
	if (word->buf)
		ft_memcpy(next, word->buf, word->len);
	return (free(word->buf), word->buf = next, word->cap = cap, 0);
}

int	exp_wordchar(t_word *word, char c)
{
	if (wordgrow(word, 1))
		return (1);
	word->buf[word->len++] = c;
	word->buf[word->len] = '\0';
	word->active = 1;
	return (0);
}

int	exp_wordstr(t_word *word, const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if (wordgrow(word, len))
		return (1);
	ft_memcpy(word->buf + word->len, str, len);
	word->len += len;
	word->buf[word->len] = '\0';
	word->active = 1;
	return (0);
}
