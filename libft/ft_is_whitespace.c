/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_is_whitespace.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rlucas <marvin@codam.nl>                     +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/05/04 13:26:10 by rlucas        #+#    #+#                 */
/*   Updated: 2020/05/04 13:35:05 by rlucas        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int		ft_is_whitespace(int c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (1);
	return (0);
}