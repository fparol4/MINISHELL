/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 12:55:12 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 12:55:12 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_pipe.h"

static int	pipe_alloc(t_command ***cmds, int **fds, size_t count)
{
	*cmds = ft_calloc(count + 1, sizeof(t_command *));
	*fds = malloc(sizeof(int) * (count - 1) * 2);
	if (!*cmds || !*fds)
		return (free(*cmds), free(*fds), 1);
	return (0);
}

int	rn_pipe(t_command *node, t_env **env)
{
	t_command	**cmds;
	int			*fds;
	size_t		count;
	size_t		idx;
	int			status;

	count = rn_pipe_count(node);
	if (count < 2)
		return (sh_err(NULL, "invalid pipe node"), 1);
	if (pipe_alloc(&cmds, &fds, count))
		return (1);
	idx = 0;
	rn_pipe_flatten(node, cmds, &idx);
	if (rn_pipe_create(fds, count - 1))
		return (free(cmds), free(fds), 1);
	status = rn_pipe_fork_wait(cmds, env, fds, count);
	free(cmds);
	free(fds);
	return (status);
}
