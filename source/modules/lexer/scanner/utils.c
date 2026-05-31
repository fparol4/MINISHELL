#include "scanner.h"

void	scanner_init(t_scanner *sc, const char *input)
{
	if (!sc || !input)
		return ;
	sc->input = (char *)input;
	sc->cursor = 0;
	sc->len = ft_strlen(sc->input);
	sc->start = 0;
}

void	scanner_advance(t_scanner *sc)
{
	if (sc && sc->input && sc->cursor < sc->len)
		sc->cursor++;
}

unsigned int	scanner_current(t_scanner *sc)
{
	if (!sc || !sc->input || sc->cursor >= sc->len)
		return (0);
	return ((unsigned int)sc->input[sc->cursor]);
}

char	scanner_peek(t_scanner *sc, size_t offset)
{
	if (!sc || !sc->input)
		return ('\0');
	if (sc->cursor + offset < sc->len)
		return (sc->input[sc->cursor + offset]);
	return ('\0');
}

int	scanner_is_end(t_scanner *sc)
{
	if (!sc || !sc->input)
		return (1);
	return (sc->cursor >= sc->len);
}

size_t	scanner_mark_start(t_scanner *sc)
{
	if (!sc || !sc->input)
		return (0);
	sc->start = sc->cursor;
	return (1);
}

char	*scanner_extract(t_scanner *sc)
{
	if (!sc || !sc->input || sc->cursor < sc->start)
		return (NULL);
	return (ft_substr(sc->input, sc->start, sc->cursor - sc->start));
}

void	scanner_take_while(t_scanner *sc, int (*f)(int))
{
	if (!sc || !sc->input || !f)
		return ;
	while (!scanner_is_end(sc) && f(scanner_current(sc)))
		scanner_advance(sc);
}

void	init_char_table(t_char_table *table)
{
	int	i;

	if (!table)
		return ;
	i = 0;
	while (i < 256)
		table->props[i++] = 0;
	i = 0;
	while (i < 256)
	{
		if (ft_isalpha(i))
			table->props[i] |= P_ALPHA;
		else if (ft_isdigit(i))
			table->props[i] |= P_DIGIT;
		else if ((i >= 9 && i <= 13) || i == 32)
			table->props[i] |= P_SPACE;
		else if (i == '\'')
			table->props[i] |= P_SQUOTE;
		else if (i == '"')
			table->props[i] |= P_DQUOTE;
		else if (i == '\\')
			table->props[i] |= P_ESCAPE;
		else
			table->props[i] |= P_SYMBOL;
		i++;
	}
}
