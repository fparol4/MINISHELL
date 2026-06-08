/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:52:15 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 18:53:02 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>

# include "../libraries/libft/libft.h"
# include "./shared.h"
# include "./env.h"
# include "./builtin.h"

# define BUFFER_SIZE 4096

# define  MSG_MINISHELL "minishell:"

#endif
