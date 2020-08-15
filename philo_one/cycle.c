/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:21:44 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/15 23:44:45 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

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
		pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
		pthread_mutex_unlock(&philo->mutex->fork[philo->next_id]);
		return (0);
	}
	pthread_mutex_lock(&philo->last_eat);
	philo->time_of_last_eat = get_time();
	pthread_mutex_unlock(&philo->last_eat);
	usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
	pthread_mutex_unlock(&philo->mutex->fork[philo->next_id]);
	if (++philo->count_eat ==\
			philo->data->num_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&philo->mutex->global_satiated);
		g_philos_satiated++;
		pthread_mutex_unlock(&philo->mutex->global_satiated);
	}
	return (1);
}

int		take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex->fork[philo->id]);
	if (!print_status(philo, TAKE_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
		return (0);
	}
	pthread_mutex_lock(&philo->mutex->fork[philo->next_id]);
	if (!print_status(philo, TAKE_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
		pthread_mutex_unlock(&philo->mutex->fork[philo->next_id]);
		return (0);
	}
	return (1);
}

void	*monitor_eat(void *philo)
{
	t_mutex *mutex;

	mutex = ((t_philo *)philo)->mutex;
	while (1)
	{
		pthread_mutex_lock(&mutex->global_died);
		if (g_philo_died)
		{
			pthread_mutex_unlock(&mutex->global_died);
			return (NULL);
		}
		pthread_mutex_unlock(&mutex->global_died);
		pthread_mutex_lock(&mutex->global_satiated);
		if (g_philos_satiated == ((t_philo *)philo)->data->num_of_philosophers)
		{
			pthread_mutex_unlock(&mutex->global_satiated);
			g_philo_died = 1;
			return (print_exit(philo, ARE_SATIATED, 1));
		}
		pthread_mutex_unlock(&mutex->global_satiated);
		usleep(10);
	}
}

void	*monitor_death(void *philo)
{
	t_philo			*p;
	unsigned long	time;

	p = philo;
	while (1)
	{
		pthread_mutex_lock(&p->last_eat);
		if ((time = get_time()) - p->time_of_last_eat >
			p->data->time_to_die)
		{
			pthread_mutex_unlock(&p->last_eat);
			pthread_mutex_lock(&p->mutex->global_died);
			if (!g_philo_died)
			{
				g_philo_died = 1;
				return (print_exit(p, HAS_DIED, time));
			}
			pthread_mutex_unlock(&p->mutex->global_died);
			return (NULL);
		}
		pthread_mutex_unlock(&p->last_eat);
		usleep(10);
	}
}
