/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _redir.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:09:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:09:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _REDIR_H
# define _REDIR_H

# include "../_runner.h"
# include <fcntl.h>

typedef enum e_heredoc_state
{
	HEREDOC_DONE,
	HEREDOC_EOF,
	HEREDOC_INTR,
	HEREDOC_FAIL
}				t_heredoc_state;

typedef struct s_redir_fd
{
	int			fd;
	int			stdio;
}				t_redir_fd;

int				rn_redir_append(char **buf, char *part);
int				rn_redir_char(char **buf, char c);
int				rn_redir_linegrow(char **line, size_t *cap, size_t len);
ssize_t			read_heredoc_loop(char **line, size_t *len, size_t *cap);
char			*rn_redir_line(char *line, t_env **env, int expand);
char			*rn_redir_readline(void);
void			rn_redir_warn(char *target);
t_heredoc_state	heredoc_read_loop(int fd, char *target, t_env **env,
					int expand);
int				rn_redir_heredoc(char *target, t_env **env, int expand);
int				rn_redir_fd(t_parser_redir *redir, t_env **env);
int				rn_redir_apply(t_parser_redir *redirs, size_t count,
					t_env **env);

#endif
