/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _path.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PATH_H
# define _PATH_H

# include "../_runner.h"
# include <sys/stat.h>
# include <unistd.h>

char	*rn_path_join(char *dir, char *cmd);
int		rn_path_match(char *full);
int		rn_path_candidate(char *full, char **fallback);

#endif
