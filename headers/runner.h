/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:53:20 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:46:08 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUNNER_H
# define RUNNER_H

# include "./minishell.h"
# include "./parser_internal.h"
# include "./sh_signal.h"

typedef struct s_arglist
{
	char	**items;
	int		size;
	int		cap;
}			t_arglist;

typedef struct s_word
{
	char	*buf;
	int		len;
	int		cap;
	int		active;
}			t_word;

char	**rn_expand(char **args, t_env **env);
int		rn_execute(t_command *cmd, t_env **env);
int		rn_exec_cmd(t_command *cmd, t_env **env);
int		rn_exec_pipe(t_command *cmd, t_env **env);
int		rn_pipe(t_command *cmd, t_env **env);
int		rn_redir_push(t_parser_redir *redirs, size_t count,
			t_env **env, int saved[2]);
int		rn_redir_restore(int saved[2]);
char	*rn_path(char **args, t_env **env);
int		rn_exec_bin(char **args, t_env **env, int *status);
int		rn_exec_ext(char **args, t_env **env);
int		rn_status_set(t_env **env, int status);
int		rn_status_get(t_env **env);

#endif
