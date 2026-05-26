#include "../../../headers/minishell.h"


static char *f_toentry(const char *key, const char *value)
{
	char *tmp;
	char *entry;

	tmp = ft_strjoin(key, "=");
	if (!entry)
		return (NULL);
	if (value)
		entry = ft_strjoin(key, value);
	else
		entry = ft_strdup(tmp);
	free(tmp);
	return (entry);
}

static int f_envsize(t_env **env)
{
	int size;
	t_env *node;

	size = 0;
	node = *env;
	while (node)
	{
		size++;
		node = node->next;
	}
	return (size);
}

static void f_arrfree(char **arr)
{
	int i;

	if (!arr) return;
	if (!*arr)
		return (free(arr));
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char **env_toarr(t_env **env)
{
	int i;
	int es;
	char **arr;
	t_env *node;

	es = f_envsize(env);
	arr = calloc(es + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	node = *env;
	i = 0;
	while (node)
	{
		arr[i] = f_toentry(node->key, node->value);
		if (!arr[i])
		{
			f_arrfree(arr);
			return (NULL);
		}
		node = node->next;
		i++;
	}
	return (NULL);
}
