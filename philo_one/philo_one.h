/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 05:11:01 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/07 12:37:01 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <pthread.h>
# include <time.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct		s_data
{
	int				num_of_philosophers;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				num_must_eat;
	int				philo_num;
	uint64_t		start;

	pthread_mutex_t	forks[num_of_philosophers];
	pthread_t		philosophers[num_of_philosophers];
	pthread_attr_t	attributes[num_of_philosophers];
}					t_data

#endif