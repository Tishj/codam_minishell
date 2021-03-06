/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   close_all.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/04 19:35:57 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/06/24 14:58:58 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

void	close_all(t_vec *fd_arr)
{
	int		*fds;
	size_t	i;

	i = 0;
	fds = (int *)fd_arr->store;
	while (i < fd_arr->index)
	{
		close(fds[i]);
		i++;
	}
}
