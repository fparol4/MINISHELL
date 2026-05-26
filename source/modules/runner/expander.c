/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"

typedef struct s_arglist
{
	char	**items;
	int		size;
	int		cap;
}			t_arglist;

typedef struct s_word
{
	char	*buf;
	int		len;
	int		cap;
	int		active;
}			t_word;

static int	f_listgrow(t_arglist *list)
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
	free(list->items);
	list->items = next;
	return (0);
}

static int	f_listadd(t_arglist *list, char *item)
{
	if (f_listgrow(list))
	{
		free(item);
		return (1);
	}
	list->items[list->size++] = item;
	list->items[list->size] = NULL;
	return (0);
}

static int	f_wordgrow(t_word *word, int add)
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
	free(word->buf);
	word->buf = next;
	word->cap = cap;
	return (0);
}

static int	f_wordchar(t_word *word, char c)
{
	if (f_wordgrow(word, 1))
		return (1);
	word->buf[word->len++] = c;
	word->buf[word->len] = '\0';
	word->active = 1;
	return (0);
}

static int	f_wordstr(t_word *word, const char *str)
{
	int	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	if (f_wordgrow(word, len))
		return (1);
	ft_memcpy(word->buf + word->len, str, len);
	word->len += len;
	word->buf[word->len] = '\0';
	word->active = 1;
	return (0);
}

static int	f_flush_word(t_arglist *list, t_word *word)
{
	char	*out;

	if (!word->active)
		return (0);
	out = ft_strdup(word->buf);
	if (!out)
		return (1);
	if (f_listadd(list, out))
		return (1);
	word->len = 0;
	word->active = 0;
	if (word->buf)
		word->buf[0] = '\0';
	return (0);
}

static int	f_add_unquoted(t_arglist *list, t_word *word, const char *value)
{
	int	i;

	i = 0;
	while (value && value[i])
	{
		if (sh_isspace(value[i]))
		{
			if (f_flush_word(list, word))
				return (1);
			while (sh_isspace(value[i]))
				i++;
		}
		else
		{
			if (f_wordchar(word, value[i++]))
				return (1);
		}
	}
	return (0);
}

static char	*f_varvalue(char *arg, int *i, t_env **env)
{
	char	*key;
	char	*value;
	int		start;

	(*i)++;
	if (arg[*i] == '?')
	{
		(*i)++;
		value = env_get(env, ENV_ERRCODE);
		if (!value)
			return ("");
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

static int	f_expand_var(t_arglist *list, t_word *word, char *arg, int *i,
		t_env **env)
{
	char	*value;

	value = f_varvalue(arg, i, env);
	if (!value)
		return (f_wordchar(word, '$'));
	return (f_add_unquoted(list, word, value));
}

static int	f_expand_quoted(t_word *word, char *arg, int *i, t_env **env)
{
	char	*value;

	value = f_varvalue(arg, i, env);
	if (!value)
		return (f_wordchar(word, '$'));
	return (f_wordstr(word, value));
}

static int	f_single(t_word *word, char *arg, int *i)
{
	(*i)++;
	word->active = 1;
	while (arg[*i] && arg[*i] != '\'')
	{
		if (f_wordchar(word, arg[*i]))
			return (1);
		(*i)++;
	}
	if (arg[*i] == '\'')
		(*i)++;
	return (0);
}

static int	f_double(t_word *word, char *arg, int *i, t_env **env)
{
	(*i)++;
	word->active = 1;
	while (arg[*i] && arg[*i] != '"')
	{
		if (arg[*i] == '$')
		{
			if (f_expand_quoted(word, arg, i, env))
				return (1);
		}
		else
		{
			if (f_wordchar(word, arg[*i]))
				return (1);
			(*i)++;
		}
	}
	if (arg[*i] == '"')
		(*i)++;
	return (0);
}

static int	rn_argexpand(char *arg, t_arglist *out, t_env **env)
{
	t_word	word;
	int		i;

	ft_bzero(&word, sizeof(t_word));
	i = 0;
	while (arg && arg[i])
	{
		if (arg[i] == '\'')
		{
			if (f_single(&word, arg, &i))
				return (free(word.buf), 1);
		}
		else if (arg[i] == '"')
		{
			if (f_double(&word, arg, &i, env))
				return (free(word.buf), 1);
		}
		else if (arg[i] == '$')
		{
			if (f_expand_var(out, &word, arg, &i, env))
				return (free(word.buf), 1);
		}
		else if (sh_isspace(arg[i]))
		{
			if (f_flush_word(out, &word))
				return (free(word.buf), 1);
			while (sh_isspace(arg[i]))
				i++;
		}
		else if (f_wordchar(&word, arg[i++]))
			return (free(word.buf), 1);
	}
	if (f_flush_word(out, &word))
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
	if (f_listgrow(&out))
	{
		sh_freeargs(out.items);
		return (NULL);
	}
	out.items[out.size] = NULL;
	return (out.items);
}
