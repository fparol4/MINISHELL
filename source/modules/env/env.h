#ifndef ENV_H
# define ENV_H

# include "../../../headers/env.h"
# include "../../../headers/errors.h"
# include "../../../headers/shared.h"

/* list.c */
t_env	*f_newnode(const char *key, const char *value);
int		f_freenode(t_env *node);

/* free.c */
int		env_free(t_env **env);

/* get.c */
char	*env_get(t_env **env, const char *key);

/* init.c */
t_env	*env_init(char **env);

/* set.c */
int		env_set(t_env **env, const char *key, const char *value);

/* size.c */
int		env_size(t_env **envm);

/* toarr.c */
char	**env_toarr(t_env **env);

/* unset.c */
int		env_unset(t_env **env, const char *key);

#endif
