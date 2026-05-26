#ifndef ENVM_COMMON_H
# define ENVM_COMMON_H

# include "../../../headers/minishell.h"

t_env	*f_newnode(const char *key, const char *value);
int		f_freenode(t_env *node);

#endif
