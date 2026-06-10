/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:46 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:46 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_runner.h"

typedef struct s_builtin_map
{
	char	*name;
	int		(*func)(char **, t_env **);
}	t_builtin_map;

static const t_builtin_map	g_map[] = {
	{"echo", bin_echo},
	{"cd", bin_cd},
	{"pwd", bin_pwd},
	{"export", bin_export},
	{"unset", bin_unset},
	{"env", bin_env},
	{"exit", bin_exit},
	{NULL, NULL}
};

int	rn_exec_bin(char **args, t_env **env, int *status)
{
	int	i;

	if (!args || !args[0])
		return (0);
	i = 0;
	while (g_map[i].name)
	{
		if (ft_strcmp(args[0], g_map[i].name) == 0)
		{
			*status = g_map[i].func(args + 1, env);
			return (1);
		}
		i++;
	}
	return (0);
}
