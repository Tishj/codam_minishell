/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ryantokens.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/05 23:24:42 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/26 12:29:22 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <libft.h>

void			move_through_quote(char *line, t_ryanlexer *lex)
{
	update_lexer(line, lex);
	while (lex->state != NORMAL)
	{
		lex->i++;
		if (check_esc_char(line, lex))
			continue ;
		update_lexer(line, lex);
	}
}

int				determine_state(char **line, size_t i, int state)
{
	if (state != ENV)
		return (WHITESPACE);
	if ((*line)[i + 1] && ft_is_whitespace((*line)[i + 1]))
		return (NORMAL);
	if (!(*line)[i + 1])
		return (NORMAL);
	return (WHITESPACE);
}

void	edit_string(char **line, size_t i, t_msh *prog, char *env_value)
{
	size_t		env_len;
	size_t		extension;
	size_t		tmp;

	env_len = ft_strclen((*line) + i + 1, ' ');
	if (ft_strclen((*line) + i + 1, '"') < env_len)
		env_len = ft_strclen((*line) + i + 1, '"');
	if (!env_value)
	{
		tmp = ft_strlen((*line)) - env_len;
		ft_memmove((*line) + i, (*line) + i + env_len,
				ft_strlen((*line) + i + env_len));
		line[tmp] = '\0';
	}
	else
	{
		if (ft_strlen(env_value) > env_len)
		{
			extension = ft_strlen(env_value) - env_len;
			if (ft_strlen((*line) + i) + extension > prog->line.cmd_len)
			{
				(*line) = ft_realloc((*line), prog->line.cmd_len + 100);
				prog->line.cmd_len += 100;
			}
			ft_printf("line b4 memmove: %s\n", (*line));
			ft_memmove((*line) + i + ft_strlen(env_value),
					(*line) + i + env_len + 1, ft_strlen((*line) + i + env_len));
			ft_memmove((*line) + i, env_value, ft_strlen(env_value));
			ft_printf("line after memmove: %s\n", (*line));
		}
	}
}

char	*env_val_get_n(const char *name, t_var *env, size_t len)
{
	t_var		*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, len) == 0)
			return (tmp->val);
		tmp = tmp->next;
	}
	return (NULL);
}

void	expand_env_value(char **line, t_msh *prog, size_t i)
{
	size_t		env_len;
	char		*env_value;

	env_len = ft_strclen((*line) + i + 1, ' ');
	if (ft_strclen((*line) + i + 1, '"') < env_len)
		env_len = ft_strclen((*line) + i + 1, '"');
	env_value = env_val_get_n((*line) + i + 1, prog->env, env_len);
	ft_printf("line in = %s\n", (*line));
	ft_printf("env_value in = %s\n", env_value);
	edit_string(line, i, prog, env_value);
}

void	evaluate_double_quotes(char **line, t_ryanlexer *lex, t_msh *prog)
{
	char		c;
	size_t		i;
	int			escape;

	if (lex->state == INSINGLEQUOTE)
		return ;
	i = lex->i + 1;
	escape = 0;
	while ((*line)[i] != '"' && escape != 1)
	{
		c = (*line)[i];
		escape = 0;
		if (c == '$')
		{
			if ((*line)[i + 1] == '(')
				(void)c; /* initiate_subshell */
			if ((*line)[i + 1] == '$')
				(void)c; /* Get process id of shell */
			if (ft_isalpha((*line)[i + 1]))
			{
				expand_env_value(line, prog, i);
				continue ;
			}
		}
		if (c == '\\')
			escape = 1;
		i++;
	}
	ft_printf("line out = %s\n", (*line));
}

size_t			sum_tokens(char **line)
{
	t_ryanlexer		lex;
	size_t			sum;

	init_lexer(&lex);
	sum = 0;
	while ((*line)[lex.i])
	{
		if (check_esc_char((*line), &lex))
			continue ;
		update_lexer((*line), &lex);
		if (lex.state == INDOUBLEQUOTE || lex.state == INSINGLEQUOTE)
		{
			sum++;
			/* evaluate_double_quotes(line, &lex, prog); */
			lex.i++;
			move_through_quote((*line), &lex);
			lex.inwhitespace = 0;
		}
		if (lex.state >= SEMICOLON && lex.state <= ENV)
			lex.state = determine_state(line, lex.i, lex.state);
		if (!lex.escape && lex.state == NORMAL && lex.prevstate == WHITESPACE)
			sum++;
		lex.escape = 0;
		lex.i++;
	}
	return (sum);
}

void		add_quote_tok(char *line, t_ryantok **tokens, t_ryanlexer *lex)
{
	if (line[lex->i] == '\'')
		lex->quotes = SINGLE;
	else if (line[lex->i] == '\"')
		lex->quotes = DOUBLE;
	line[lex->i] = '\0';
	lex->i++;
	create_token(line + lex->i, (*tokens) + lex->tokeni, lex);
	move_through_quote(line, lex);
	lex->quotes = NORMAL;
	line[lex->i] = '\0';
	lex->i++;
}

void		add_env_tok(char *line, t_ryanlexer *lex, t_ryantok **tokens)
{
	if (!line[lex->i + 1] || ft_is_whitespace(line[lex->i + 1]))
		create_token(line + lex->i, (*tokens) + lex->tokeni, lex);
	else
	{
		lex->env = 1;
		create_token(line + lex->i + 1, (*tokens) + lex->tokeni, lex);
		while (line[lex->i] && !ft_is_whitespace(line[lex->i]) &&
				!ft_strchr("><|&", line[lex->i]))
			lex->i++;
		lex->state = WHITESPACE;
	}
}

void		state_action(char *line, t_ryanlexer *lex, t_ryantok **tokens)
{
	if (lex->state == SEMICOLON)
	{
		lex->nexttype = COMMAND;
		lex->command_present = 0;
		lex->cmd_num++;
		lex->state = WHITESPACE;
	}
	if (lex->state == IREDIRECT)
		lex->nexttype = INPUT_SENDER;
	if (lex->state == OREDIRECT)
		lex->nexttype = WRITEFILE;
	if (lex->state == OAPPEND)
		lex->nexttype = APPENDFILE;
	if (lex->state == PIPE_PIPE)
	{
		lex->nexttype = PIPEDCOMMAND;
		lex->command_present = 0;
		lex->pipe = 1;
		lex->cmd_num++;
	}
	if (lex->state == ENV)
		add_env_tok(line, lex, tokens);
}

void		gen_tokens(char *line, t_ryantok **tokens)
{
	t_ryanlexer		lex;

	init_lexer(&lex);
	while (line[lex.i])
	{
		if (check_esc_char(line, &lex))
			continue ;
		update_lexer(line, &lex);
		if (lex.state == INDOUBLEQUOTE || lex.state == INSINGLEQUOTE)
		{
			add_quote_tok(line, tokens, &lex);
			continue ;
		}
		if (lex.state >= SEMICOLON && lex.state <= ENV)
			state_action(line, &lex, tokens);
		if (lex.state == NORMAL && lex.prevstate == WHITESPACE)
			create_token(line + lex.i, (*tokens) + lex.tokeni, &lex);
		if (lex.state == WHITESPACE)
			line[lex.i] = '\0';
		lex.escape = 0;
		lex.i++;
	}
	(*tokens)[lex.tokeni].value = NULL;
}

void		remove_escapes(char *line)
{
	size_t		i;
	size_t		len;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\\')
		{
			len = ft_strlen(line);
			ft_memmove(line + i, line + i + 1, ft_strlen(line + i + 1));
			line[len - 1] = '\0';
		}
		i++;
	}
}

t_ryantok	*tokenizer(char **line, t_msh *prog)
{
	t_ryantok		*tokens;
	size_t			totaltokens;

    (void)prog;
	totaltokens = sum_tokens(line);
	ft_printf("sum = %u\n", totaltokens);
	tokens = (t_ryantok *)malloc(sizeof(t_ryantok) * (totaltokens + 1));
	gen_tokens(*line, &tokens);
	return (tokens);
}
