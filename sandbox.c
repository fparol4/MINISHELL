#include "./libraries/libft/libft.h"
#include <complex.h>
#include <errno.h>
#define println(...) printf(__VA_ARGS__), printf("\n")

#include <stdio.h>

void main()
{
	char buff[1024];

	char *cwd = getcwd(buff, sizeof(buff));
}
