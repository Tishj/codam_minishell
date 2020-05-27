/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ryantokens.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/05 23:24:42 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/27 22:33:20 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void		assign_token_indexes(char *line, t_ryantok *tokens)
{
	size_t		i;

	i = 0;
	while (tokens[i].value)
	{
		tokens[i].value = line + tokens[i].index;
		i++;
	}
}

t_ryantok	*tokenizer(t_msh *prog)
{
	t_ryantok		*tokens;
	size_t			totaltokens;

	totaltokens = sum_tokens(prog->line.cmd);
	ft_printf("sum = %u\n", totaltokens);
	tokens = (t_ryantok *)malloc(sizeof(t_ryantok) * (totaltokens + 1));
	gen_tokens(&tokens, prog);
	assign_token_indexes(prog->line.cmd, tokens);
	print_tokens(tokens);
    free(tokens);
	/* return (tokens); */
	return (NULL);
}
