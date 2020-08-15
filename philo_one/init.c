/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:21:11 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/16 00:20:15 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		init_data(char **argv, t_data *data)
{
	if (!check_arguments(argv))
		return (exit_error(MSG_ERROR_ARGV));
	data->num_of_philosophers = ft_atoul(argv[1]);
	if (data->num_of_philosophers < 2)
		return (exit_error(MSG_ERROR_PHILO));
	data->num_of_times_each_philosopher_must_eat =\
		(argv[5]) ? ft_atoul(argv[5]) : -1;
	data->time_to_die = ft_atoul(argv[2]);
	data->time_to_eat = ft_atoul(argv[3]) * 1000;
	data->time_to_sleep = ft_atoul(argv[4]) * 1000;
	return (EXIT_SUCCESS);
}

int		init_thread(t_philo **philo, t_mutex *mutex, int num)
{
	int			i;

	mutex->fork = NULL;
	if (!(mutex->fork = malloc(sizeof(pthread_mutex_t) * num)))
		return (exit_error(MSG_ERROR_MALLOC));
	if (!(*philo = malloc(sizeof(t_philo) * num)))
		return (exit_error(MSG_ERROR_MALLOC));
	i = 0;
	while (i < num)
	{
		pthread_mutex_init(&mutex->fork[i], NULL);
		pthread_mutex_init(&(*philo)[i].last_eat, NULL);
		i++;
	}
	pthread_mutex_init(&mutex->write, NULL);
	pthread_mutex_init(&mutex->global_died, NULL);
	pthread_mutex_init(&mutex->global_satiated, NULL);
	return (EXIT_SUCCESS);
}
