#include "../../headers/minishell.h"

void	sh_err(char *scope, char *msg)
{
	ft_putstr_fd(MSG_MINISHELL, 2);
	if (scope)
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(scope, 2);
		ft_putstr_fd(":", 2);
	}
	if (msg)
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(msg, 2);
	}
	ft_putstr_fd("\n", 2);
}

void	sh_err2(char *scope, char *arg, char *msg)
{
	ft_putstr_fd(MSG_MINISHELL, 2);
	if (scope)
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(scope, 2);
		ft_putstr_fd(":", 2);
	}
	if (arg)
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(":", 2);
	}
	if (msg)
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(msg, 2);
	}
	ft_putstr_fd("\n", 2);
}
