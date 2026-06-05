/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:53:35 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/05 09:47:50 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SH_SIGNAL_H
# define SH_SIGNAL_H

# include <signal.h>
# include <readline/readline.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_signal;

typedef enum e_sig_mode
{
	SIG_INTERACTIVE,
	SIG_EXEC,
	SIG_HEREDOC
}	t_sig_mode;

void	sh_sig_mode(t_sig_mode mode);

#endif
