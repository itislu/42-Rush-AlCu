/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstto_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:17:31 by ldulling          #+#    #+#             */
/*   Updated: 2025/03/02 10:11:10 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

void	**ft_lstto_array(t_list **lst)
{
	void	**array;
	size_t	i;

	if (lst == NULL || *lst == NULL)
		return (NULL);
	array = (void **)malloc((ft_lstsize(*lst) + 1) * sizeof(void *));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (*lst)
	{
		array[i] = ft_lstpop_front_content(lst);
		i++;
	}
	array[i] = NULL;
	return (array);
}
