/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/08 14:24:50 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/08 14:34:53 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		ft_atoi(const char *nptr)
{
	int	i;
	int	is_n;
	int	num;

	i = 0;
	is_n = 1;
	num = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			is_n *= (-1);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + nptr[i] - '0';
		i++;
	}
	return (num * is_n);
}

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr_unsigned_long(long nb)
{
	if (nb > 9)
	{
		ft_putnbr_unsigned_long(nb / 10);
		ft_putnbr_unsigned_long(nb % 10);
	}
	else
		ft_putchar(nb + '0');
}
