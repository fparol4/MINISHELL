#include "scanner.h"

void	scanner_advance(t_scanner *sc)
{
	if (sc && sc->input && sc->cursor < sc->len)
		sc->cursor++;
}

int	scanner_isend(t_scanner *sc)
{
	if (!sc || !sc->input)
		return (1);
	return (sc->cursor >= sc->len);
}

size_t	scanner_markstart(t_scanner *sc)
{
	if (!sc || !sc->input)
		return (0);
	sc->start = sc->cursor;
	return (1);
}

void	scanner_until(t_scanner *sc, int (*f)(int))
{
	if (!sc || !sc->input || !f)
		return ;
	while (!scanner_isend(sc) && f(scanner_current(sc)))
		scanner_advance(sc);
}
