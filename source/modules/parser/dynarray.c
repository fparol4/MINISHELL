#include "../../headers/parser_internal.h"

void	*dynarray_append(void *arr, size_t elem_size, size_t *count)
{
	void	*new;
	size_t	old;

	old = 0;
	if (count)
		old = *count;
	new = ft_calloc(old + 2, elem_size);
	if (!new)
		return (NULL);
	if (arr)
	{
		ft_memcpy(new, arr, old * elem_size);
		free(arr);
	}
	if (count)
		(*count)++;
	return (new);
}
