/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 22:03:16 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/15 23:30:56 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

uint64_t	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int			check_arguments(char **argv)
{
	int i;
	int j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int			exit_error(char *s)
{
	write(2, s, ft_strlen(s));
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

void		clean(t_philo *philo)
{
	int i;

	i = -1;
	while (++i < philo->data->num_of_philosophers)
	{
		pthread_mutex_destroy(&philo->mutex->fork[i]);
		pthread_mutex_destroy(&philo[i].last_eat);
	}
	pthread_mutex_destroy(&philo->mutex->write);
	pthread_mutex_destroy(&philo->mutex->global_died);
	pthread_mutex_destroy(&philo->mutex->global_satiated);
	free(philo->mutex->fork);
	free(philo);
}
