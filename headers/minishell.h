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
# define MSG_MINISHELL "minishell:"

extern volatile sig_atomic_t	g_signal;

typedef enum e_sig_mode
{
	SIG_INTERACTIVE,
	SIG_EXEC,
	SIG_HEREDOC
}								t_sig_mode;

typedef struct s_shell
{
	t_env						*env;
	t_bool						running;
}								t_shell;

int								core_run(char **envp);
int								core_init(t_shell *shell, char **envp);
int								core_loop(t_shell *shell);
void							core_destroy(t_shell *shell);

int								sh_isspace(char c);
int								sh_varstart(char c);
int								sh_varchar(char c);
int								sh_isidentifier(char *s);
int								sh_has_quotes(char *s);
char							*sh_quote_remove(char *s);
void							sh_freesec(void **items);
void							sh_freeargs(char **args);
void							sh_err(char *scope, char *msg);
void							sh_err2(char *scope, char *arg, char *msg);
void							sh_stxerr(t_syntax_error type);
void							sh_sig_mode(t_sig_mode mode);

#endif
