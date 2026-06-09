/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strbuilder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:47 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:47 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

int	rn_redir_append(char **buf, char *part)
{
	char	*next;

	next = ft_strjoin(*buf, part);
	free(*buf);
	*buf = next;
	return (!next);
}

int	rn_redir_char(char **buf, char c)
{
	char	part[2];

	part[0] = c;
	part[1] = '\0';
	return (rn_redir_append(buf, part));
}
