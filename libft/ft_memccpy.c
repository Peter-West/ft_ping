/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppellegr <ppellegr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2013/11/23 18:17:42 by ppellegr          #+#    #+#             */
/*   Updated: 2013/11/23 18:17:47 by ppellegr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *s1, const void *s2, int c, size_t n)
{
	char		*c1;
	char		*c2;

	c1 = (char *)s1;
	c2 = (char *)s2;	
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	while (n--)
	{
		*c1 = *c2;
		if (*c2 == c)
			return ((void *)++c1);
		c1++;
		c2++;
	}
	return (NULL);
}
