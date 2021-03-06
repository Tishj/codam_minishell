/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_exit.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 23:02:16 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/06/29 13:38:40 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <msh_builtin.h>

static int	str_isdigit(char *str, int *n)
{
	size_t	i;

	i = 0;
	if (!str[i])
	{
		*n = -1;
		return (0);
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			*n = -1;
			return (0);
		}
		i++;
	}
	return (1);
}

int			ft_exit(t_msh *prog, int argc, char **argv)
{
	int n;

	n = 0;
	if (argc == 2)
		n = ft_atoi(argv[1]);
	if (argc > 2)
	{
		ft_printf("msh: exit: too many arguments to exit.\n");
		return (1);
	}
	if (argc >= 2 && !str_isdigit(argv[1], &n))
		ft_printf("msh: exit: numeric argument reguired.\n");
	std_exit(prog, (n % 256));
	return (0);
}
