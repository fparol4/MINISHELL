/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:44 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:44 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_bin.h"

static char	*export_key(char *arg)
{
	char	*c_eq;
	char	*key;

	c_eq = ft_strchr(arg, '=');
	if (c_eq)
	{
		key = ft_substr(arg, 0, c_eq - arg);
		return (key);
	}
	key = ft_strdup(arg);
	return (key);
}

static int	export_invalid(char *arg)
{
	ft_putstr_fd("minishell: export: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

static int	export_arg(char *arg, t_env **env)
{
	char	*key;
	char	*c_eq;
	int		status;

	status = 0;
	c_eq = ft_strchr(arg, '=');
	key = export_key(arg);
	if (!key || !sh_isidentifier(key))
		status = export_invalid(arg);
	else if (c_eq)
		env_set(env, key, c_eq + 1);
	else
		env_set(env, key, NULL);
	free(key);
	return (status);
}

int	bin_export(char **args, t_env **env)
{
	int	i;
	int	code;
	int	status;

	code = 0;
	if (!args[0])
	{
		status = export_print(env);
		return (status);
	}
	i = 0;
	while (args[i])
	{
		if (export_arg(args[i], env))
			code = 1;
		i++;
	}
	return (code);
}
