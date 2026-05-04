# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <unistd.h>

int	builtin_env(char **args, t_env **env_list)
{
	t_env	*cur;

	(void)args;
	cur = *env_list;
	while (cur)
	{
		if (cur->value)
		{
			write(1, cur->key, ft_strlen(cur->key));
			write(1, "=", 1);
			write(1, cur->value, ft_strlen(cur->value));
			write(1, "\n", 1);
		}
		cur = cur->next;
	}
	return (0);
}
