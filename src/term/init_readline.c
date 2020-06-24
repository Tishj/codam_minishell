/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_readline.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 17:17:50 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/24 14:29:30 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <stdlib.h>
#include <termcap.h>
#include <termios.h>
#include <libft.h>

void		init_readline(t_msh *prog)
{
	prog->line = (t_line){0};
	prog->line.prompt = prompt(prog, &prog->line);
	if (!prog->line.prompt)
		error_exit(prog, MEM_FAIL);
}
