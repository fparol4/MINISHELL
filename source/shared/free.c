#include "../../headers/shared.h"

void	sh_freesec(void **items)
{
	int	i;

	if (!items)
		return ;
	i = 0;
	while (items[i])
		free(items[i++]);
	free(items);
}

void	sh_freeargs(char **args)
{
	sh_freesec((void **)args);
}
