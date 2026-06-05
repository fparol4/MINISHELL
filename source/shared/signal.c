/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/sh_signal.h"

volatile sig_atomic_t	g_signal = 0;

static void	sh_sigint_interactive(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	sh_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

static void	sig_mode_interactive(struct sigaction *sa_int,
		struct sigaction *sa_quit)
{
	sa_int->sa_handler = sh_sigint_interactive;
	sa_quit->sa_handler = SIG_IGN;
}

static void	sig_mode_exec(struct sigaction *sa_int,
		struct sigaction *sa_quit)
{
	sa_int->sa_handler = SIG_IGN;
	sa_quit->sa_handler = SIG_IGN;
}

static void	sig_mode_heredoc(struct sigaction *sa_int,
		struct sigaction *sa_quit)
{
	sa_int->sa_handler = sh_sigint_heredoc;
	sa_quit->sa_handler = SIG_IGN;
}

void	sh_sig_mode(t_sig_mode mode)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	if (mode == SIG_INTERACTIVE)
		sig_mode_interactive(&sa_int, &sa_quit);
	else if (mode == SIG_EXEC)
		sig_mode_exec(&sa_int, &sa_quit);
	else
		sig_mode_heredoc(&sa_int, &sa_quit);
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}
