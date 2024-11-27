/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sinawara <sinawara@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:24:05 by sinawara          #+#    #+#             */
/*   Updated: 2024/10/15 12:24:05 by sinawara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_hex(unsigned int n)
{
	int		len;
	char	c;

	len = 0;
	if (n >= 16)
		len += ft_print_hex(n / 16);
	n = n % 16;
	if (n < 10)
		c = n + '0';
	else
		c = n + 'a' - 10;
	write(1, &c, 1);
	len++;
	return (len);
}