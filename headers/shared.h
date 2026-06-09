/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 00:00:00 by Codex             #+#    #+#             */
/*   Updated: 2026/06/09 00:00:00 by Codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
# define SHARED_H

# include "./core.h"
# include "./parser.h"

void	sh_freesec(void **items);
void	sh_freeargs(char **args);
void	sh_err(char *scope, char *msg);
void	sh_err2(char *scope, char *arg, char *msg);
void	sh_stxerr(t_syntax_error type);
void	sh_sig_mode(t_sig_mode mode);
int		sh_isspace(char c);
int		sh_varstart(char c);
int		sh_varchar(char c);
int		sh_isidentifier(char *s);
int		sh_has_quotes(char *s);
char	*sh_quote_remove(char *s);

#endif
