#include "../../../headers/minishell.h"

int	env_size(t_env **envm)
{
	int		size;
	t_env	*node;

	if (!envm)
		return (0);
	size = 0;
	node = *envm;
	while (node)
	{
		size++;
		node = node->next;
	}
	return (size);
}
