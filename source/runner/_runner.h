/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _runner.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _RUNNER_H
# define _RUNNER_H

# include "../../headers/minishell.h"

char	**rn_expand(char **args, t_env **env);
char	*exp_varvalue(char *arg, int *i, t_env **env);
int		rn_execute(t_command *cmd, t_env **env, int heredoc_fd, t_ast *ast);
int		rn_pipe(t_command *cmd, t_env **env, t_ast *ast);
int		rn_redir_push(t_parser_redir *redirs, size_t count, t_env **env,
			int saved[2], int heredoc_fd);
int		rn_redir_restore(int saved[2]);
int		rn_exec_bin(char **args, t_env **env, int *status);
int		rn_exec_ext(char **args, t_env **env);
int		rn_status_set(t_env **env, int status);
int		rn_status_get(t_env **env);
char	*rn_path(char **args, t_env **env);

#endif
