#include "../../headers/shared.h"

int	sh_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	sh_varstart(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	sh_varchar(char c)
{
	return (ft_isalnum(c) || c == '_');
}
