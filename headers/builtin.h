#ifndef BUILTIN_H
#define BUILTIN_H

#include "env.h"

#define PWD_HOME "HOME"

int bin_echo(char **args, t_env **env);
int bin_cd(char **args, t_env **env);
int bin_pwd(char **args, t_env **env);
int bin_exit(char **args, t_env **env);
int bin_env(char **args, t_env **env);
int bin_export(char **args, t_env **env);
int bin_unset(char **args, t_env **env);

#endif
