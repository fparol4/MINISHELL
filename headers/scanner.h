#ifndef SCANNER_H
# define SCANNER_H

# include "../libraries/libft/libft.h"

typedef enum e_capture_status
{
	CAPTURE_FAIL,
	CAPTURE_OK,
	CAPTURE_UNCLOSED_GROUP
}	t_capture_status;

typedef enum e_char_prop
{
	P_NONE = 0,
	P_ALPHA = 1 << 0,
	P_DIGIT = 1 << 1,
	P_SPACE = 1 << 2,
	P_SQUOTE = 1 << 3,
	P_DQUOTE = 1 << 4,
	P_ESCAPE = 1 << 5,
	P_SYMBOL = 1 << 6
}	t_char_prop;

typedef struct s_scanner
{
	char	*input;
	size_t	cursor;
	size_t	len;
	size_t	start;
}	t_scanner;

typedef struct s_char_table
{
	unsigned int	props[256];
}	t_char_table;

void			scanner_init(t_scanner *sc, const char *input);
void			scanner_advance(t_scanner *sc);
unsigned int	scanner_current(t_scanner *sc);
char			scanner_peek(t_scanner *sc, size_t offset);
int				scanner_isend(t_scanner *sc);
size_t			scanner_markstart(t_scanner *sc);
void			scanner_until(t_scanner *sc, int (*f)(int));
char			*scanner_extract(t_scanner *sc);
void			scanner_tablechar(t_char_table *table);
void			scanner_tabletoken(t_char_table *table);

#endif
