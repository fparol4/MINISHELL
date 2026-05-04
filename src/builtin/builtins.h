# ifndef BUILTINS_H
#  define BUILTINS_H

# include "../types/envm.h"

int	builtin_echo(char **args, t_env **env_list);
int	builtin_cd(char **args, t_env **env_list);
int	builtin_pwd(char **args, t_env **env_list);
int	builtin_export(char **args, t_env **env_list);
int	builtin_unset(char **args, t_env **env_list);
int	builtin_env(char **args, t_env **env_list);
int	builtin_exit(char **args, t_env **env_list);

# endif
