#ifndef RUNNER_H
# define RUNNER_H

# include "./minishell.h"

char	**rn_expand(char **args, t_env **env);
int		rn_execute(t_exnode *node, t_env **env);
int		rn_exec_cmd(t_exnode *node, t_env **env);
int		rn_exec_pipe(t_exnode *node, t_env **env);
int		rn_pipe(t_exnode *node, t_env **env);
int		rn_redir_push(t_redir *redir, t_env **env, int saved[2]);
int		rn_redir_restore(int saved[2]);
char	*rn_path(char **args, t_env **env);
int		rn_exec_bin(char **args, t_env **env, int *status);
int		rn_exec_ext(char **args, t_env **env);
int		rn_status_set(t_env **env, int status);
int		rn_status_get(t_env **env);

#endif
