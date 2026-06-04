/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"
#include "../../../headers/runner.h"
#include <limits.h>

static int	exit_value(char *arg, long long *out)
{
	unsigned long long	value;
	unsigned long long	limit;
	int					sign;

	while (sh_isspace(*arg))
		arg++;
	sign = 1;
	if (*arg == '-' || *arg == '+')
	{
		if (*arg++ == '-')
			sign = -1;
	}
	if (!ft_isdigit(*arg))
		return (0);
	value = 0;
	limit = (unsigned long long)LLONG_MAX;
	if (sign < 0)
		limit++;
	while (ft_isdigit(*arg))
	{
		if (value > (limit - (*arg - '0')) / 10)
			return (0);
		value = value * 10 + (*arg++ - '0');
	}
	while (sh_isspace(*arg))
		arg++;
	if (*arg)
		return (0);
	if (sign < 0 && value == limit)
		*out = LLONG_MIN;
	else if (sign < 0)
		*out = -(long long)value;
	else
		*out = (long long)value;
	return (1);
}

int	bin_exit(char **args, t_env **env)
{
	long long	code;

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", 1);
	if (!args || !args[0])
		exit(rn_status_get(env));
	if (!exit_value(args[0], &code))
	{
		sh_err2("exit", args[0], "numeric argument required");
		env_set(env, ENV_ERRCODE, "2");
		exit(2);
	}
	if (args[1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		env_set(env, ENV_ERRCODE, "1");
		return (1);
	}
	exit((unsigned char)code);
}
