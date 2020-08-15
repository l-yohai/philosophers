/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:20:40 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/16 02:55:08 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	print_msg(t_philo *philo, int index, uint64_t time)
{
	ft_putnbr(time - philo->data->time_of_start);
	write(1, "\t", 1);
	ft_putnbr(philo->id + 1);
	if (index == TAKE_FORK)
		write(1, MSG_TAKE_FORK, ft_strlen(MSG_TAKE_FORK));
	else if (index == IS_EATING)
		write(1, MSG_EAT, ft_strlen(MSG_EAT));
	else if (index == IS_SLEEPING)
		write(1, MSG_SLEEP, ft_strlen(MSG_SLEEP));
	else if (index == IS_THINKING)
		write(1, MSG_THINK, ft_strlen(MSG_THINK));
}

int		print_status(t_philo *philo, int index)
{
	pthread_mutex_lock(&philo->mutex->global_died);
	if (g_philo_died)
	{
		pthread_mutex_unlock(&philo->mutex->global_died);
		return (0);
	}
	pthread_mutex_unlock(&philo->mutex->global_died);
	pthread_mutex_lock(&philo->mutex->write);
	print_msg(philo, index, get_time());
	pthread_mutex_unlock(&philo->mutex->write);
	return (1);
}

void	*print_exit(t_philo *philo, int index, uint64_t time)
{
	pthread_mutex_lock(&philo->mutex->write);
	pthread_mutex_unlock(&philo->mutex->global_died);
	if (index == HAS_DIED)
	{
		ft_putnbr(time - philo->data->time_of_start);
		write(1, "\t", 1);
		ft_putnbr(philo->id + 1);
		write(1, MSG_DIE, ft_strlen(MSG_DIE));
	}
	if (index == ARE_SATIATED)
	{
		write(1, MSG_FINISH, ft_strlen(MSG_FINISH));
		exit(EXIT_SUCCESS);
	}
	pthread_mutex_unlock(&philo->mutex->write);
	return (NULL);
}
