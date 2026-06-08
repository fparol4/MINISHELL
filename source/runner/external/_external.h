/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _external.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:15 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:15 by fcardozo         ###   ########.fr       */
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
}			t_ext_kind;

t_ext_kind	rn_ext_classify(char *path);
int			rn_ext_report(char *path, t_ext_kind kind);
int			rn_ext_execfail(char *path);
int			rn_ext_resolve(char **args, t_env **env, char **path);
void		rn_ext_child(char *path, char **args, char **envp);
int			rn_ext_wait(pid_t pid);

#endif
