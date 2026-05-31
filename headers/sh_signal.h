#ifndef SH_SIGNAL_H
# define SH_SIGNAL_H

# include <signal.h>

extern volatile sig_atomic_t	g_signal;

typedef enum e_sig_mode
{
	SIG_INTERACTIVE,
	SIG_EXEC,
	SIG_HEREDOC
}	t_sig_mode;

void	sh_sig_mode(t_sig_mode mode);

#endif
