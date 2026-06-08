/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:38 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:38 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_shared.h"

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

void	sh_stxerr(t_syntax_error type)
{
	if (type == SNTX_UNCLOSED_QUOTE)
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
	else if (type == SNTX_PIPE_START)
		ft_putstr_fd("minishell: syntax error: pipe at start\n", 2);
	else if (type == SNTX_PIPE_END)
		ft_putstr_fd("minishell: syntax error: pipe at end\n", 2);
	else if (type == SNTX_EMPTY_CMD)
		ft_putstr_fd("minishell: syntax error: empty command\n", 2);
	else if (type == SNTX_REDIR_NO_TARGET)
		ft_putstr_fd("minishell: syntax error: redirection target\n", 2);
}
