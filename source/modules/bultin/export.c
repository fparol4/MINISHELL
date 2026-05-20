#include "../../../headers/minishell.h"

static int f_vid(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void f_sortkeys(char **keys, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(keys[j], keys[j + 1]) > 0)
			{
				tmp = keys[j];
				keys[j] = keys[j + 1];
				keys[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int f_print(t_env **env)
{
	int	i;
	int size;
	t_env *node;
	t_env *c_node;
	char **keys;

	size = env_size(env);
	keys = ft_calloc(size + 1, sizeof(char));
	if (!keys)
		return (1);
	i = 0;
	c_node = *env;
	while (c_node)
	{
		keys[i++] = c_node->key;
		c_node = c_node->next;
	}
	f_sortkeys(keys)
}

int bin_export(char **args, t_env **env)
{
	int i;
	int err;
	char *key;
	char *c_eq;

	if (!args[0])
		return (f_list(env));
	i = 0;
	while (args[i])
	{
		if (!f_vid(args[i]))
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			i++;
			err = 1;
		}
		else
		{
			c_eq = ft_strchr(args[i], '=');
			if (c_eq)
			{
				key = ft_substr(args[i], 0, c_eq - args[i]);
				env_set(env, key, c_eq + 1);
				free(key);
			}
			else
				env_set(env, args[i], NULL);
		}
		i++;
	}
}
