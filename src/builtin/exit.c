# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <unistd.h>
# include <stdlib.h>

static int	is_numeric(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_env **env_list)
{
	int	code;

	(void)env_list;
	write(1, "exit\n", 5);
	if (!args[1])
		exit(0);
	if (!is_numeric(args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		exit(2);
	}
	if (args[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	code = ft_atoi(args[1]) & 0xFF;
	exit(code);
}
