/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"

#define TEST_SHARED_RUNNER
#include "runner/command.c"
#include "runner/expand.c"
#include "runner/pipe.c"
#include "runner/redir.c"
#include "runner/signal.c"
#include "runner/status.c"
#include "shared.c"

snow_main();
