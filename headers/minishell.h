/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libraries/libft/libft.h"
# include "./bin.h"
# include "./env.h"
# include "./lexer.h"
# include "./parser.h"
# include "./runner.h"
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>

# define BUFFER_SIZE 4096
# define PROMPT "GA&Fabricio:minishell$ "

extern volatile sig_atomic_t	g_signal;

typedef enum e_sig_mode
{
	SIG_INTERACTIVE,
	SIG_EXEC,
	SIG_HEREDOC
}	t_sig_mode;

typedef struct s_shell
{
	t_env	*env;
	t_bool	running;
}	t_shell;

int	core_run(char **envp);

#endif
