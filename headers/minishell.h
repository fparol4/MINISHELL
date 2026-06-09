/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* external */
# include "../libraries/libft/libft.h"
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>

/* internal */
# include "./bin.h"
# include "./core.h"
# include "./env.h"
# include "./lexer.h"
# include "./parser.h"
# include "./runner.h"
# include "./shared.h"

#endif
