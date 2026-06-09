/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _pipe.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:46 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:46 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PIPE_H
# define _PIPE_H

# include "../_runner.h"
# include <sys/wait.h>

typedef struct s_pipe_ctx
{
	t_command	**cmds;
	t_env		**env;
	int			*fds;
	size_t		pipe_count;
}				t_pipe_ctx;

size_t			rn_pipe_count(t_command *node);
void			rn_pipe_flatten(t_command *node, t_command **items,
					size_t *idx);
void			rn_pipe_close_all(int *fds, size_t pipe_count);
int				rn_pipe_create(int *fds, size_t pipe_count);
void			rn_pipe_child(t_pipe_ctx *ctx, size_t pos);
int				rn_pipe_wait(pid_t *pids, size_t count);
int				rn_pipe_fork_wait(t_command **cmds, t_env **env, int *fds,
					size_t count);

#endif
