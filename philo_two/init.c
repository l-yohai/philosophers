/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:21:11 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/16 05:21:00 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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

int		init_semaphore(t_philo **philo, t_semaphore *sem, int num)
{
	int			i;
	char		*s;

	if (!(*philo = malloc(sizeof(t_philo) * num)))
		return (exit_error(MSG_ERROR_MALLOC));
	sem->fork = sem_open("fork", O_CREAT | O_EXCL, 0644, num);
	sem->write = sem_open("write", O_CREAT | O_EXCL, 0644, 1);
	sem->global_died = sem_open("global_died", O_CREAT | O_EXCL, 0644, 1);
	sem->global_satiated =\
		sem_open("global_satiated", O_CREAT | O_EXCL, 0644, 1);
	i = 0;
	while (i < num)
	{
		s = ft_strjoin("last_eat", ft_ultoa((unsigned long)i));
		(*philo)[i].last_eat = sem_open(s, O_CREAT | O_EXCL, 0644, 1);
		free(s);
		if ((*philo)[i].last_eat == SEM_FAILED)
			return (exit_error(MSG_ERROR_SEM));
		i++;
	}
	unlink_semaphores(num);
	if (sem->fork == SEM_FAILED || sem->write == SEM_FAILED ||\
		sem->global_died == SEM_FAILED || sem->global_satiated == SEM_FAILED)
		return (exit_error(MSG_ERROR_SEM));
	return (EXIT_SUCCESS);
}

int		unlink_semaphores(int num)
{
	int		i;
	char	*s;

	i = 0;
	while (i < num)
	{
		s = ft_strjoin("last_eat", ft_ultoa((unsigned long)i));
		sem_unlink(s);
		free(s);
		i++;
	}
	sem_unlink("fork");
	sem_unlink("write");
	sem_unlink("global_died");
	sem_unlink("global_satiated");
	return (1);
}
