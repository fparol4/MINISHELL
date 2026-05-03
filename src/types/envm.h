# ifndef ENVM_H
#  define ENVM_H

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*envm_init(char **envp);
char	*envm_get(t_env *env_list, const char *key);
int		envm_set(t_env **env_list, const char *key, const char *value);
void	envm_unset(t_env **env_list, const char *key);
char	**envm_toarr(t_env *env_list);
void	envm_free(t_env **env_list);

# endif
