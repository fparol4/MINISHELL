# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <unistd.h>
# include <stdlib.h>

static int	is_valid_id(const char *s)
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

static int	list_len(t_env *list)
{
	int	n;

	n = 0;
	while (list)
	{
		n++;
		list = list->next;
	}
	return (n);
}

static void	sort_keys(char **keys, int n)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strncmp(keys[j], keys[j + 1], ft_strlen(keys[j]) + 1) > 0)
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

static int	print_sorted(t_env *env_list)
{
	int		n;
	int		i;
	char	**keys;
	t_env	*cur;
	t_env	*node;

	n = list_len(env_list);
	keys = malloc(sizeof(char *) * (n + 1));
	if (!keys)
		return (1);
	cur = env_list;
	i = 0;
	while (cur)
	{
		keys[i++] = cur->key;
		cur = cur->next;
	}
	keys[i] = NULL;
	sort_keys(keys, n);
	i = 0;
	while (keys[i])
	{
		node = env_list;
		while (node && ft_strncmp(node->key, keys[i], ft_strlen(keys[i]) + 1))
			node = node->next;
		write(1, "declare -x ", 11);
		write(1, keys[i], ft_strlen(keys[i]));
		if (node && node->value)
		{
			write(1, "=\"", 2);
			write(1, node->value, ft_strlen(node->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		i++;
	}
	free(keys);
	return (0);
}

int	builtin_export(char **args, t_env **env_list)
{
	int		i;
	int		ret;
	char	*eq;
	char	*key;

	if (!args[1])
		return (print_sorted(*env_list));
	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_id(args[i]))
		{
			write(2, "minishell: export: `", 20);
			write(2, args[i], ft_strlen(args[i]));
			write(2, "': not a valid identifier\n", 26);
			ret = 1;
			i++;
			continue ;
		}
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			key = ft_substr(args[i], 0, eq - args[i]);
			envm_set(env_list, key, eq + 1);
			free(key);
		}
		else
			envm_set(env_list, args[i], NULL);
		i++;
	}
	return (ret);
}
