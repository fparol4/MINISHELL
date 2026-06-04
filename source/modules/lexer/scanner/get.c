#include "header.h"

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

char	*scanner_extract(t_scanner *sc)
{
	if (!sc || !sc->input || sc->cursor < sc->start)
		return (NULL);
	return (ft_substr(sc->input, sc->start, sc->cursor - sc->start));
}
