#include "../../headers/lexer_internal.h"

t_token_type	define_type(char *type)
{
	if (ft_strncmp(type, ">>", 2) == 0)
		return (TOKEN_APPEND);
	if (ft_strncmp(type, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	if (ft_strncmp(type, "|", 1) == 0)
		return (TOKEN_PIPE);
	if (ft_strncmp(type, "<", 1) == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strncmp(type, ">", 1) == 0)
		return (TOKEN_REDIR_OUT);
	return (TOKEN_NONE);
}
