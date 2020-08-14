/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 00:52:10 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/15 01:00:35 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

int g_philo_died;
int g_philos_satiated;

typedef struct		s_mutex
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	write;
	pthread_mutex_t global_died;
	pthread_mutex_t global_satiated;
}					t_mutex;

typedef struct		s_rules
{
	int				number_of_philosophers;
	int				number_of_times_each_philosopher_must_eat;
	unsigned long	time_of_start;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
}					t_rules;

typedef struct		s_data
{
	int				id;
	int				next_id;
	pthread_t		tid;
	int				meal_counter;
	unsigned long	time_of_last_meal_ms;
	pthread_mutex_t	last_meal;
	t_mutex			*mutex;
	t_rules			*rules;
}					t_data;

#endif
