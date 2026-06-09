/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:58 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:58 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_bin.h"
#include <limits.h>

static int	exit_parse_prefix(char **arg, int *sign)
{
	while (sh_isspace(**arg))
		(*arg)++;
	*sign = 1;
	if (**arg == '-' || **arg == '+')
	{
		if (*(*arg)++ == '-')
			*sign = -1;
	}
	if (!ft_isdigit(**arg))
		return (0);
	return (1);
}

static int	exit_parse_digits(char **arg, unsigned long long *value, int sign)
{
	unsigned long long	limit;

	*value = 0;
	limit = (unsigned long long)LLONG_MAX;
	if (sign < 0)
		limit++;
	while (ft_isdigit(**arg))
	{
		if (*value > (limit - (**arg - '0')) / 10)
			return (0);
		*value = *value * 10 + (*(*arg)++ - '0');
	}
	while (sh_isspace(**arg))
		(*arg)++;
	if (**arg)
		return (0);
	return (1);
}

static long long	exit_apply_sign(unsigned long long value, int sign)
{
	if (sign < 0 && value == (unsigned long long)LLONG_MAX + 1)
		return (LLONG_MIN);
	if (sign < 0)
		return (-(long long)value);
	return ((long long)value);
}

static int	exit_value(char *arg, long long *out)
{
	int					sign;
	unsigned long long	value;

	if (!exit_parse_prefix(&arg, &sign))
		return (0);
	if (!exit_parse_digits(&arg, &value, sign))
		return (0);
	*out = exit_apply_sign(value, sign);
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
