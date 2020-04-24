/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tc_funcs1.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/04/20 10:59:21 by rlucas        #+#    #+#                 */
/*   Updated: 2020/04/24 20:30:44 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <minishell.h>
#include <termcap.h>
#include <termios.h>

/*
** ft_realloc() creates a new string, copies the contents from a previous
** string, the frees the previous string.
*/


static char	*ft_realloc(char *str, int change)
{
	char	*new;

	new = (char *)ft_calloc(ft_strlen(str) + change + 1, 1);
	if (!new)
	{
		free(str);
		return (NULL);
	}
	ft_strlcpy(new, str, ft_strlen(str) + change + 1);
	free(str);
	return (new);
}

/*
** refresh() will delete all of the current terminal output so far, using the
** values stored for row position, then reprint everything. This isn't a
** very efficient thing to do, and this function should NOT be used in a final
** program.
*/

void	refresh(t_line *line)
{
	int		i;

	i = 0;
	while (i < line->cursor.row)
	{
		tputs(tgetstr("dl", NULL), 1, &ft_putchar);
		tputs(tgetstr("up", NULL), 1, &ft_putchar);
		i++;
	}
	tputs(tgetstr("cr", NULL), 1, &ft_putchar);
	ft_printf("%s%s", line->prompt, line->cmd);
	tputs(tgoto(tgetstr("cm", NULL), line->cursor.col, line->cursor.row),
			1, &ft_putchar);
}

/*
** add_char() will reallocate a new length for the command string, then
** add the newly input character to it.
*/

void	insert_char(t_line *line, char c)
{
	int		arraypos;

	arraypos = line->max.col * line->cursor.row + line->cursor.col -
		line->promptlen;
	ft_memmove(line->cmd + arraypos + 1, line->cmd + arraypos,
			ft_strlen(line->cmd + arraypos));
	line->cmd[arraypos] = c;
	line->cmd[line->cmd_len + 1] = '\0';
}

int		add_char(t_line *line, char buf[6])
{
	int		i;

	line->cmd = ft_realloc(line->cmd, 1);
	if (!line->cmd)
		return (-1);
	insert_char(line, buf[0]);
	line->cmd_len++;
	line->cursor.col++;
	if (line->cursor.col >= line->max.col)
	{
		line->cursor.col = 0;
		line->cursor.row++;
		line->scroll = SCROLLDOWN;
	}
	tputs(tgetstr("im", NULL), 1, &ft_putchar);
	ft_putchar(buf[0]);
	tputs(tgetstr("ei", NULL), 1, &ft_putchar);
	line->total_rows = (line->cmd_len + line->promptlen) /
		(line->max.col);
	i = (line->cursor.col == 0) ? line->cursor.row : line->cursor.row + 1;
	while (i <= line->total_rows)
	{
		tputs(tgoto(tgetstr("cm", NULL),
				0, i), 1, &ft_putchar);
		tputs(tgetstr("im", NULL), 1, &ft_putchar);
		ft_putchar(line->cmd[(i) * (line->max.col) - line->promptlen]);
		tputs(tgetstr("ei", NULL), 1, &ft_putchar);
		i++;
	}
	return (0);
}

/*
** delete_char() will reallocate a new length for the command string, and
** remove the last character from it.
*/

void	remove_char(t_line *line)
{
	int		arraypos;

	arraypos = line->max.col * line->cursor.row + line->cursor.col -
		line->promptlen - 1;
	ft_memmove(line->cmd + arraypos, line->cmd + arraypos + 1,
			ft_strlen(line->cmd + arraypos + 1));
	line->cmd[line->cmd_len] = '\0';
}

int		delete_char(t_line *line)
{
	int		i;

	if (line->cmd_len == 0)
		return (0);
	if (line->cursor.row == 0 && line->cursor.col == (int)line->promptlen)
		return (0);
	remove_char(line);
	line->cmd = ft_realloc(line->cmd, -1);
	line->cursor.col--;
	line->cmd_len--;
	if (!line->cmd)
		return (-1);
	if (line->cursor.col < 0)
	{
		line->cursor.col = line->max.col - 1;
		line->cursor.row--;
		tputs(tgetstr("sr", NULL), 1, &ft_putchar);
	}
	tputs(tgoto(tgetstr("cm", NULL),
				line->cursor.col, line->cursor.row), 1, &ft_putchar);
	tputs(tgoto(tgetstr("dc", NULL), 1, 0), 1, &ft_putchar);
	i = line->cursor.row;
	while (i < line->total_rows)
	{
		tputs(tgoto(tgetstr("cm", NULL),
				line->max.col, i), 1, &ft_putchar);
		/* tputs(tgetstr("im", NULL), 1, &ft_putchar); */
		ft_putchar(line->cmd[(i + 1) * (line->max.col) - line->promptlen - 1]);
		/* tputs(tgetstr("ei", NULL), 1, &ft_putchar); */
		tputs(tgoto(tgetstr("cm", NULL),
				0, i + 1), 1, &ft_putchar);
		tputs(tgoto(tgetstr("dc", NULL), 1, 0), 1, &ft_putchar);
		i++;
	}
	return (0);
}

/*
** cursor_left() moves the cursor one position to the left
*/

int		cursor_left(t_line *line)
{
	if (line->cursor.row == 0 && line->cursor.col <= (int)line->promptlen)
		return (0);
	line->cursor.col--;
	if (line->cursor.col < 0)
	{
		line->cursor.col = line->max.col - 1;
		line->cursor.row--;
		tputs(tgetstr("sr", NULL), 1, &ft_putchar);
	}
	tputs(tgoto(tgetstr("cm", NULL),
				line->cursor.col, line->cursor.row), 1, &ft_putchar);
	return (0);
}

int		cursor_right(t_line *line)
{
	if (line->cursor.row * line->max.col + line->cursor.col >=
			(int)(line->promptlen + line->cmd_len))
		return (0);
	line->cursor.col++;
	if (line->cursor.col >= line->max.col)
	{
		line->cursor.col = 0;
		line->cursor.row++;
		line->scroll = SCROLLDOWN;
	}
	tputs(tgoto(tgetstr("cm", NULL),
				line->cursor.col, line->cursor.row), 1, &ft_putchar);
	return (0);
}
