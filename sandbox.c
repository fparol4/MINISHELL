#include "./libraries/libft/libft.h"
#include <errno.h>
#define println(...) printf(__VA_ARGS__), printf("\n")

#include <stdio.h>

void main()
{
	errno = 1;
	perror("open");
}
