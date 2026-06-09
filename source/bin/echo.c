/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:44 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:44 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_bin.h"

static int	echo_has_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] == '\0')
		return (0);
	i = 1;
	while (arg[i])
		if (arg[i++] != 'n')
			return (0);
	return (1);
}

static void	echo_print_args(char **args, int start)
{
	int	i;

	i = start;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
}

int	bin_echo(char **args, t_env **env)
{
	int	i;
	int	nl;

	(void)env;
	if (!args)
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	i = 0;
	nl = 1;
	while (args[i] && echo_has_flag(args[i]))
	{
		nl = 0;
		i++;
	}
	echo_print_args(args, i);
	if (nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
