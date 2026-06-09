/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _core.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:09:15 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:09:15 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CORE_H
# define _CORE_H

# include "../../headers/minishell.h"

int		core_run(char **envp);
int		core_init(t_shell *shell, char **envp);
int		core_loop(t_shell *shell);
void	core_destroy(t_shell *shell);
int		rn_status_set(t_env **env, int status);
int		rn_status_get(t_env **env);

#endif
