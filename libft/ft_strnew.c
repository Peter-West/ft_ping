/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppellegr <ppellegr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/11/26 17:57:53 by ppellegr          #+#    #+#             */
/*   Updated: 2013/11/26 17:57:55 by ppellegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char	*allo;
	char	*tmp;
	size_t	size_tmp;

	size_tmp = size + 1;
	allo = malloc(size + 1);
	if (allo == NULL)
		return (NULL);
	tmp = allo;
	while (--size_tmp)
		*allo++ = '\0';
	tmp[size] = '\0';
	return (tmp);
}
