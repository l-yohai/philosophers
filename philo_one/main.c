/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 14:16:34 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/07 12:41:58 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr(const char *s)
{
	write(1, s, ft_strlen(s));
}

void	ft_putchar(const char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	if (n >= 0)
	{
		if (n > 9)
			ft_putnbr(n / 10);
		ft_putchar(n % 10 + '0');
	}
	else
	{
		ft_putchar('-');
		if (n < -9)
			ft_putnbr(n / (-10));
		ft_putchar((n % 10 * -1) + '0');
	}
}

uint64_t get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	think(t_data *data)
{
	ft_putnbr(get_time() - data->start);
	ft_putnbr(data->philo_num));
	ft_putstr(" is thinking\n");
}

void	eat(t_data *data)
{
	ft_putnbr(get_time() - data->start);
	ft_putnbr(data->philo_num));
	ft_putstr(" is eating\n");
}

void	sleep(t_data *data)
{
	ft_putnbr(get_time() - data->start);
	ft_putnbr(data->philo_num));
	ft_putstr(" is sleeping\n");
}

void	pick_up(t_data *data)
{
	ft_putnbr(get_time() - data->start);
	ft_putnbr(data->philo_num));
	ft_putstr(" has taken a fork");
}

int	behave(t_data *data)
{
	while (1)
	{
		think(data);
		pick_up(data);
		eat(data);
		// put_down(data);
		sleep(data);
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (-1);
	data.start = get_time();
	data.num_of_philosophers = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	data.num_must_eat = (argc == 6) ? ft_atoi(argv[5]) : 0;
	data.philo_num = 0;
	while (data.philo_num < data.num_of_philosophers))
	{
		pthread_mutex_init(&data.forks[data.philo_num], NULL);
		data.philo_num++;
	}
	data.philo_num = 0;
	while (data.philo_num < data.num_of_philosophers))
	{
		pthread_attr_init(&data.attributes[data.philo_num]);
		data.philo_num++;
	}
	data.philo_num = 0;
	while (data.philo_num < data.num_of_philosophers))
	{
		pthread_create(&data.philosophers[data.philo_num], &data.attributes[data.philo_num], behave, &data);
		data.philo_num++;
	}
	data.philo_num = 0;
	while (data.philo_num < data.num_of_philosophers))
	{
		pthread_join(&data.philosophers[data.philo_num], NULL);
		data.philo_num++;
	}
	return (0);
}