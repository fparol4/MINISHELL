/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tester.h"

describe(sh_sig_mode)
{
	{
	it("initializes g_signal to 0")
	{
		g_signal = 99;
		g_signal = 0;
		asserteq(g_signal, 0);
	}
	}
	{
	struct sigaction	sa;

	it("SIG_INTERACTIVE: SIGQUIT is ignored, SIGINT has a custom handler")
	{
		sh_sig_mode(SIG_INTERACTIVE);
		sigaction(SIGQUIT, NULL, &sa);
		assert(sa.sa_handler == SIG_IGN);
		sigaction(SIGINT, NULL, &sa);
		assert(sa.sa_handler != SIG_IGN && sa.sa_handler != SIG_DFL);
	}
	}
	{
	struct sigaction	sa;

	it("SIG_EXEC: both SIGINT and SIGQUIT are ignored")
	{
		sh_sig_mode(SIG_EXEC);
		sigaction(SIGINT, NULL, &sa);
		assert(sa.sa_handler == SIG_IGN);
		sigaction(SIGQUIT, NULL, &sa);
		assert(sa.sa_handler == SIG_IGN);
	}
	}
	{
	struct sigaction	sa;

	it("SIG_HEREDOC: SIGQUIT ignored, SIGINT has heredoc handler")
	{
		sh_sig_mode(SIG_HEREDOC);
		sigaction(SIGQUIT, NULL, &sa);
		assert(sa.sa_handler == SIG_IGN);
		sigaction(SIGINT, NULL, &sa);
		assert(sa.sa_handler != SIG_IGN && sa.sa_handler != SIG_DFL);
	}
	}
}
