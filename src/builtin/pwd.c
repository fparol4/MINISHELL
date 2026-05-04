# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <unistd.h>

int	builtin_pwd(char **args, t_env **env_list)
{
	char	buf[4096];
	char	*fallback;

	(void)args;
	if (getcwd(buf, sizeof(buf)))
	{
		write(1, buf, ft_strlen(buf));
		write(1, "\n", 1);
		return (0);
	}
	fallback = envm_get(*env_list, "PWD");
	if (fallback)
	{
		write(1, fallback, ft_strlen(fallback));
		write(1, "\n", 1);
	}
	return (1);
}
