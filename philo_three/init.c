/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:21:11 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/20 18:59:07 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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

int		init_semaphore(t_semaphore *sem, int num)
{
	unlink_semaphores(num);
	sem->fork = sem_open("fork", O_CREAT | O_EXCL, 0644, num);
	sem->write = sem_open("write", O_CREAT | O_EXCL, 0644, 1);
	sem->finished_eats = sem_open("finished_eats", O_CREAT | O_EXCL, 0644, 0);
	sem->simulation_end =\
		sem_open("simulation_end", O_CREAT | O_EXCL, 0644, 0);
	if (sem->fork == SEM_FAILED || sem->write == SEM_FAILED ||\
		sem->finished_eats == SEM_FAILED || sem->simulation_end == SEM_FAILED)
		return (unlink_semaphores(num));
	return (EXIT_SUCCESS);
}

int		unlink_semaphores(int num)
{
	int		i;
	char	*s;

	i = 0;
	while (i < num)
	{
		s = ft_strjoin("last_eat", ft_ultoa(i));
		sem_unlink(s);
		free(s);
		i++;
	}
	sem_unlink("fork");
	sem_unlink("write");
	sem_unlink("finished_eats");
	sem_unlink("simulation_end");
	return (EXIT_FAILURE);
}
