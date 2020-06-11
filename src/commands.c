/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   commands.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/29 19:22:44 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/06/11 14:49:22 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include "libft.h"
#include <stdio.h>

void	print_command(t_cmd *command)
{
	size_t				i;
	static const char	*commandtypes[] = {
	[EOC] = "END_OF_COMMAND",
	[PIPE] = "PIPE",
	[APPEND] = "APPEND",
	[TRUNC] = "TRUNC",
	[IN_REDIR] = "IN_REDIR",
	[DEFAULT] = "DEFAULT"
	};

	i = 0;
	dprintf(2, "type: %s\n", commandtypes[command->cmdtype]);
	while (command->args[i])
	{
		dprintf(2, "%s%c", command->args[i], command->args[i + 1] ? ' ' : '\n');
		i++;
	}
	/* sleep(5); */
}

char	**ft_str2clear(char **str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

int		is_pipe(int *types)
{
	int	i;

	i = 0;
	while (types[i] != SEPARATOR)
	{
		if (types[i] == PIPEDCOMMAND)
			return (1);
		i++;
	}
	return (0);
}

t_cmd	*new_command(char **argstart, int *types, t_vec *fd_arr)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmdpipe[0] = -1;
	new->cmdpipe[1] = -1;
	new->cmdtype = (is_pipe(types) ? PIPEDCOMMAND : COMMAND);
	if (new->cmdtype == PIPEDCOMMAND)
	{
		if (pipe(new->cmdpipe) == -1)
		{
			free(new);
			return (NULL); //error
		}
		vec_add(fd_arr, &new->cmdpipe[0]);
		vec_add(fd_arr, &new->cmdpipe[1]);
	}
	new->argtypes = types;
	new->args = argstart;
	new->next = NULL;
	return (new);
}

t_cmd	*push_command(t_cmd **commands, t_cmd *new)
{
	t_cmd	*iter;

	if (!new)
		return (NULL);
	if (!*commands)
	{
		*commands = new;
		return (new);
	}
	iter = *commands;
	while (iter && iter->next)
		iter = iter->next;
	iter->next = new;
	return (new);
}

int		clear_commands(t_cmd *commands)
{
	t_cmd	*iter;
	t_cmd	*del;

	iter = commands;
	while (iter)
	{
		del = iter;
		iter = iter->next;
		free(del);
	}
	return (0);
}

int			get_commands(t_msh *prog, t_ryantok *tokens, size_t totaltokens)
{
	size_t	i;
	int		cmd;

	i = 0;
	cmd = 0;
	if (!conv_tokens(prog, tokens, totaltokens))
		return (0);
	if (!vec_new(&prog->file_arr, sizeof(int)))
		return (0);
	prog->commands = NULL;
	while (i < totaltokens)
	{
		if (cmd == tokens[i].cmd_num)
		{
			if (!push_command(&prog->commands,
				new_command((char **)prog->args.store + i + cmd,
				(int *)prog->argtypes.store + i + cmd, &prog->file_arr)))
				return (clear_commands(prog->commands));
			cmd++;
		}
		i++;
	}
	return (1);
}
