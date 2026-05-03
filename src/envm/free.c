# include "../types/envm.h"
# include <stdlib.h>

void	envm_free(t_env **env_list)
{
	t_env	*node;
	t_env	*next;

	if (!env_list || !*env_list)
		return ;
	node = *env_list;
	while (node)
	{
		next = node->next;
		free(node->key);
		free(node->value);
		free(node);
		node = next;
	}
	*env_list = NULL;
}
