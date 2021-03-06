/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_unset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/17 23:13:43 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/06/18 12:31:11 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <msh_env.h>

void	env_unset(t_var **env, char *name)
{
	t_var	*iter;
	t_var	*last;

	iter = *env;
	last = iter;
	if (!*env)
		return ;
	if (ft_strcmp(name, (*env)->name) == 0)
	{
		*env = (*env)->next;
		env_del(iter);
		return ;
	}
	while (iter)
	{
		if (ft_strcmp(name, iter->name) == 0)
		{
			last->next = iter->next;
			env_del(iter);
			iter = last;
		}
		last = iter;
		iter = iter->next;
	}
}
