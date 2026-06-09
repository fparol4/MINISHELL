/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:00:00 by Codex             #+#    #+#             */
/*   Updated: 2026/06/09 00:00:00 by Codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

# include "../libraries/libft/libft.h"
# include <signal.h>
# include "./env.h"

# define BUFFER_SIZE 4096
# define PROMPT "GA&Fabricio:minishell$ "

extern volatile sig_atomic_t	g_signal;

typedef enum e_sig_mode
{
	SIG_INTERACTIVE,
	SIG_EXEC,
	SIG_HEREDOC
}	t_sig_mode;

typedef enum e_shell_status
{
	SH_STATUS_SYNTAX_ERROR = 2,
	SH_STATUS_SIGINT = 130
}	t_shell_status;

typedef struct s_shell
{
	t_env	*env;
	t_bool	running;
}	t_shell;

int	core_run(char **envp);

#endif
