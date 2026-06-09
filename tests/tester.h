/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTER_H
# define TESTER_H

# include "../headers/minishell.h"
# include "../source/runner/_runner.h"
# include "snow.h"
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

/*
Suggested rn_expand tests:
- single quotes remove quotes and keep literal $VAR
- double quotes remove quotes and expand $VAR
- unquoted $VAR expands and splits spaces
- quoted $VAR expands and does not split spaces
- unknown variables become empty
- empty quoted args stay as empty args
- $ alone stays literal
- invalid var start keeps $ literal
- $? expands from ENV_ERRCODE
- mixed quoted/unquoted parts join into the same word
*/

#endif
