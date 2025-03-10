/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_d.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 16:04:18 by ldulling          #+#    #+#             */
/*   Updated: 2025/01/29 10:06:59 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

/**
 * Can prepend a whole list, not just one node.
 */
void	ft_lstadd_front_d(t_list_d **lst, t_list_d *new)
{
	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		ft_lstadd_back_d(&new, *lst);
		*lst = new;
	}
}
