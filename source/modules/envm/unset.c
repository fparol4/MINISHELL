#include "../../../headers/minishell.h"
#include "./common.c"

int env_unset(t_env **env, const char *key)
{
	t_env *prev;
	t_env *node;
	size_t key_l;

	if (!env || !*env || !key)
		return (0);
	prev = NULL;
	node = *env;
	while (node) {
		if (ft_strcmp(node->key, key) == 0) {
			if (prev)
				prev->next = node->next;
			else
				*env = node->next;
			f_freenode(node);
			return (0);
		}
		prev = node;
		node = node->next;
	}
	return (0);
}
