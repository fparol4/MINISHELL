/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:47 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:47 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	sh_err(char *scope, char *msg)
{
	ft_putstr_fd("minishell:", STDERR_FILENO);
	if (scope)
	{
		ft_putstr_fd(" ", STDERR_FILENO);
		ft_putstr_fd(scope, STDERR_FILENO);
		ft_putstr_fd(":", STDERR_FILENO);
	}
	if (msg)
	{
		ft_putstr_fd(" ", STDERR_FILENO);
		ft_putstr_fd(msg, STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	sh_err2(char *scope, char *arg, char *msg)
{
	ft_putstr_fd("minishell:", STDERR_FILENO);
	if (scope)
	{
		ft_putstr_fd(" ", STDERR_FILENO);
		ft_putstr_fd(scope, STDERR_FILENO);
		ft_putstr_fd(":", STDERR_FILENO);
	}
	if (arg)
	{
		ft_putstr_fd(" ", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(":", STDERR_FILENO);
	}
	if (msg)
	{
		ft_putstr_fd(" ", STDERR_FILENO);
		ft_putstr_fd(msg, STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	sh_stxerr(t_syntax_error type)
{
	if (type == SNTX_UNCLOSED_QUOTE)
		ft_putstr_fd("minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
	else if (type == SNTX_PIPE_START)
		ft_putstr_fd("minishell: syntax error: pipe at start\n", STDERR_FILENO);
	else if (type == SNTX_PIPE_END)
		ft_putstr_fd("minishell: syntax error: pipe at end\n", STDERR_FILENO);
	else if (type == SNTX_EMPTY_CMD)
		ft_putstr_fd("minishell: syntax error: empty command\n", STDERR_FILENO);
	else if (type == SNTX_REDIR_NO_TARGET)
		ft_putstr_fd("minishell: syntax error: redirection target\n",
			STDERR_FILENO);
}
