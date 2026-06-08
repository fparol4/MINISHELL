/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:53:42 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 18:53:42 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHARED_H
# define SHARED_H

# include "../libraries/libft/libft.h"

void	sh_freesec(void **items);
void	sh_freeargs(char **args);
void	sh_err(char *scope, char *msg);
void	sh_err2(char *scope, char *arg, char *msg);
int		sh_isspace(char c);
int		sh_varstart(char c);
int		sh_varchar(char c);
int		sh_isidentifier(char *s);
int		sh_has_quotes(char *s);
char	*sh_quote_remove(char *s);

#endif
