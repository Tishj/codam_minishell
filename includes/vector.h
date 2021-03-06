/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbruinem <tbruinem@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/06/17 14:44:25 by tbruinem      #+#    #+#                 */
/*   Updated: 2020/06/17 14:45:17 by tbruinem      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <sys/types.h>

typedef	struct		s_vec
{
	char			*store;
	size_t			type_size;
	size_t			capacity;
	size_t			index;
}					t_vec;

int					vec_add(t_vec *vector, void *buffer);
int					vec_new(t_vec *vector, size_t type_size);
int					vec_destroy(t_vec *vector, void (*del)(void *));
int					vec_get(t_vec *vector, void *buffer, size_t index);

#endif
