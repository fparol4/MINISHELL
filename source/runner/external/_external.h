/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _external.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EXTERNAL_H
# define _EXTERNAL_H

# include "../_runner.h"
# include <errno.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>

typedef enum e_ext_kind
{
	EXT_READY,
	EXT_NOT_FOUND,
	EXT_DENIED,
	EXT_DIR
}	t_ext_kind;

int	rn_ext_resolve(char **args, t_env **env, char **path);

#endif
