/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft_types.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/29 20:03:02 by rlucas        #+#    #+#                 */
/*   Updated: 2020/06/02 11:05:28 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_TYPES_H
# define LIBFT_TYPES_H

# include <stddef.h>

typedef struct		s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct		s_gnlopts
{
	int				ret;
	int				more;
	char			*buf;
}					t_gnlopts;

typedef struct		s_vecstr
{
	char			*str;
	size_t			capacity;
	size_t			len;
}					t_vecstr;

#endif
