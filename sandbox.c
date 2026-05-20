#include "./libraries/libft/libft.h"
#define println(...) printf(__VA_ARGS__), printf("\n")

#include <stdio.h>

void main()
{
	char *s1 = "C";
	char *s2 = "B";
	int c = ft_strcmp(s1, s2);
	println("c::%d\n", c);
}
