/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_abs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldulling <ldulling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 01:16:45 by ldulling          #+#    #+#             */
/*   Updated: 2025/02/12 20:12:48 by ldulling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * The ft_abs function calculates the absolute value of a signed integer.
 *
 * @param n    The signed integer to get the absolute value of.
 *
 * @return     Returns the absolute value as an unsigned integer.
 *
 * @note       Be aware that for LONG_MIN, the absolute value would exceed 
 *             LONG_MAX in two's complement, which is why the return type is
 *             unsigned.
 */
unsigned long	ft_abs(long n)
{
	if (n < 0)
		return (-(unsigned long)n);
	return (n);
}
