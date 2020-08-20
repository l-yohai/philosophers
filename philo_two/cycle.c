/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:21:44 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/18 09:23:14 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	*cycle(void *philo)
{
	t_philo		*p;
	pthread_t	thread;

	p = philo;
	p->next_id = (p->id == p->data->num_of_philosophers - 1) ? 0 : p->id + 1;
	p->count_eat = 0;
	p->time_of_last_eat = p->data->time_of_start;
	pthread_create(&thread, NULL, monitor_death, p);
	while (1)
	{
		if (!eat(p))
			break ;
		if (!print_status(p, IS_SLEEPING))
			break ;
		usleep(p->data->time_to_sleep);
		if (!print_status(p, IS_THINKING))
			break ;
	}
	pthread_join(thread, NULL);
	return (NULL);
}

int		eat(t_philo *philo)
{
	if (!take_forks(philo))
		return (0);
	if (!print_status(philo, IS_EATING))
	{
		sem_post(philo->semaphore->fork);
		sem_post(philo->semaphore->fork);
		return (0);
	}
	sem_wait(philo->last_eat);
	philo->time_of_last_eat = get_time();
	sem_post(philo->last_eat);
	usleep(philo->data->time_to_eat);
	sem_post(philo->semaphore->fork);
	sem_post(philo->semaphore->fork);
	if (++philo->count_eat ==\
			philo->data->num_of_times_each_philosopher_must_eat)
	{
		sem_wait(philo->semaphore->global_satiated);
		g_philos_satiated++;
		sem_post(philo->semaphore->global_satiated);
	}
	return (1);
}

int		take_forks(t_philo *philo)
{
	sem_wait(philo->semaphore->fork);
	if (!print_status(philo, TAKE_FORK))
	{
		sem_post(philo->semaphore->fork);
		return (0);
	}
	sem_wait(philo->semaphore->fork);
	if (!print_status(philo, TAKE_FORK))
	{
		sem_post(philo->semaphore->fork);
		sem_post(philo->semaphore->fork);
		return (0);
	}
	return (1);
}

void	*monitor_eat(void *philo)
{
	t_semaphore *sem;

	sem = ((t_philo *)philo)->semaphore;
	while (1)
	{
		sem_wait(sem->global_died);
		if (g_philo_died)
		{
			sem_post(sem->global_died);
			return (NULL);
		}
		sem_post(sem->global_died);
		sem_wait(sem->global_satiated);
		if (g_philos_satiated == ((t_philo *)philo)->data->num_of_philosophers)
		{
			sem_post(sem->global_satiated);
			g_philo_died = 1;
			return (print_exit(philo, ARE_SATIATED, 0));
		}
		sem_post(sem->global_satiated);
		usleep(10);
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
		if ((time = get_time()) - p->time_of_last_eat >
			p->data->time_to_die)
		{
			sem_post(p->last_eat);
			sem_wait(p->semaphore->global_died);
			if (!g_philo_died)
			{
				g_philo_died = 1;
				return (print_exit(p, HAS_DIED, time));
			}
			sem_post(p->semaphore->global_died);
			return (NULL);
		}
		sem_post(p->last_eat);
		usleep(10);
	}
}
