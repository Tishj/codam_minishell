/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_export.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 23:02:16 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/06/17 15:56:07 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <msh_builtin.h>
#include <msh_env.h>

void	ft_export(t_msh *prog, int argc, char **argv)
{
	size_t	i;
	size_t	tmp;

	if (argc == 1)
		return ;
	i = 1;
	while (argv[i])
	{
		tmp = ft_strclen(argv[i], '=');
		if (tmp < ft_strlen(argv[i]))
		{
			argv[i][tmp] = '\0';
			(void)env_val_set(argv[i], prog->env, &argv[i][tmp + 1]);
		}
		i++;
	}
	env_update(prog);
}
