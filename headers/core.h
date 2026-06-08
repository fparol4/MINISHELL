/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:12:34 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:12:34 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

# include "./minishell.h"

typedef struct s_shell
{
	t_env	*env;
	t_bool	running;
}	t_shell;

int		core_run(char **envp);
int		core_init(t_shell *shell, char **envp);
int		core_loop(t_shell *shell);
void	core_destroy(t_shell *shell);

#endif
