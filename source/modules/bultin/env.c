#include "../../../headers/minishell.h"

int bin_env(char **args, t_env **env)
{
	t_env *item;
	(void) args;

	item = *env;
	while (item)
	{
		if (item->value)
		{
			ft_putstr_fd(item->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(item->value, 1);
		}
		item = item->next;
	}
	return (0);
}
