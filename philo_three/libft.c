/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:26:15 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/18 08:58:15 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

size_t			ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void			ft_putchar(char c)
{
	write(1, &c, 1);
}

void			ft_putnbr(unsigned long n)
{
	if (n > 9)
		ft_putnbr(n / 10);
	ft_putchar(n % 10 + '0');
}

unsigned long	ft_atoul(const char *s)
{
	size_t			i;
	unsigned long	num;

	i = 0;
	num = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		num = num * 10 + s[i] - '0';
		i++;
	}
	return (num);
}

char			*ft_ultoa(unsigned long n)
{
	int				len;
	unsigned long	temp;
	char			*result;
	int				i;

	len = 1;
	temp = n;
	while (temp >= 10)
	{
		temp /= 10;
		len++;
	}
	if (!(result = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	temp = n;
	while (i < len)
	{
		result[len - i - 1] = temp % 10 + '0';
		temp /= 10;
		i++;
	}
	result[len] = 0;
	return (result);
}
