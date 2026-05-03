/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 10:22:41 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/03 13:03:03 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_string(const char *s, char c);
static char		*next_string(const char **s, char c, size_t *len);
static void		free_split(char **list_split, size_t len_split);

char	**ft_split(const char *s, char c)
{
	size_t	len_split;
	size_t	len;
	size_t	number_words;
	char	*start;
	char	**list_split;

	number_words = count_string(s, c);
	list_split = malloc((number_words + 1) * (sizeof(char *)));
	if (!list_split)
		return (NULL);
	len_split = 0;
	while (number_words-- > 0)
	{
		len = 0;
		start = next_string(&s, c, &len);
		list_split[len_split] = ft_substr(start, 0, len);
		if (!(list_split[len_split]))
		{
			free_split(list_split, len_split);
			return (NULL);
		}
		len_split++;
	}
	list_split[len_split] = (NULL);
	return (list_split);
}

static void	free_split(char	**list_split, size_t len_split)
{
	while (len_split > 0)
	{
		free(list_split[len_split - 1]);
		len_split--;
	}
	free(list_split);
}

static char	*next_string(const char **s, char c, size_t *len)
{
	const char	*start;

	while (**s == c)
		(*s)++;
	start = *s;
	while ((**s != c) && (**s != '\0'))
	{
		(*len)++;
		(*s)++;
	}
	return ((char *)start);
}

static size_t	count_string(const char *s, char c)
{
	size_t	delimiter;
	size_t	count_str;

	delimiter = 0;
	count_str = 0;
	while (s[delimiter] != '\0')
	{
		while (s[delimiter] == c)
			delimiter++;
		if (s[delimiter] == '\0')
			return (count_str);
		count_str++;
		while ((s[delimiter] != c) && (s[delimiter] != '\0'))
			delimiter++;
	}
	return (count_str);
}
