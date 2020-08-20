/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:21:44 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/20 17:26:48 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	cycle(t_philo *philo)
{
	pthread_t	thread;
	char		*s;

	philo->next_id =\
		(philo->id == philo->data->num_of_philosophers - 1) ? 0 : philo->id + 1;
	philo->count_eat = 0;
	philo->semaphore->fork = sem_open("fork", 0644);
	philo->semaphore->write = sem_open("write", 0644);
	philo->semaphore->finished_eats = sem_open("finished_eats", 0644);
	philo->semaphore->simulation_end = sem_open("simulation_end", 0644);
	s = ft_strjoin("last_eat", ft_ultoa(philo->id));
	philo->last_eat = sem_open(s, 0644);
	free(s);
	philo->time_of_last_eat = philo->data->time_of_start;
	pthread_create(&thread, NULL, monitor_death, philo);
	while (1)
	{
		eat(philo);
		print_status(philo, IS_SLEEPING);
		usleep(philo->data->time_to_sleep);
		print_status(philo, IS_THINKING);
	}
}

void	eat(t_philo *philo)
{
	sem_wait(philo->semaphore->fork);
	print_status(philo, TAKE_FORK);
	sem_wait(philo->semaphore->fork);
	print_status(philo, TAKE_FORK);
	print_status(philo, IS_EATING);
	sem_wait(philo->last_eat);
	philo->time_of_last_eat = get_time();
	sem_post(philo->last_eat);
	usleep(philo->data->time_to_eat);
	sem_post(philo->semaphore->fork);
	sem_post(philo->semaphore->fork);
	if (++philo->count_eat ==\
			philo->data->num_of_times_each_philosopher_must_eat)
		sem_post(philo->semaphore->finished_eats);
}

void	monitor_eat(void *philo)
{
	t_philo	*p;
	int		count;

	p = philo;
	count = 0;
	while (1)
	{
		sem_wait(p->semaphore->finished_eats);
		if (++count >= p->data->num_of_philosophers)
		{
			print_exit(philo, ARE_SATIATED, 1);
			exit(0);
		}
	}
}

void	*monitor_death(void *philo)
{
	t_philo		*p;
	uint64_t	time;

	p = philo;
	while (1)
	{
		sem_wait(p->last_eat);
		if ((time = get_time()) - p->time_of_last_eat > p->data->time_to_die)
		{
			sem_post(p->last_eat);
			print_exit(p, HAS_DIED, time);
			return (NULL);
		}
		sem_post(p->last_eat);
		usleep(10);
	}
}
