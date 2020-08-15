/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 23:20:40 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/16 05:09:56 by yohlee           ###   ########.fr       */
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
	sem_wait(philo->semaphore->global_died);
	if (g_philo_died)
	{
		sem_post(philo->semaphore->global_died);
		return (0);
	}
	sem_post(philo->semaphore->global_died);
	sem_wait(philo->semaphore->write);
	print_msg(philo, index, get_time());
	sem_post(philo->semaphore->write);
	return (1);
}

void	*print_exit(t_philo *philo, int index, uint64_t time)
{
	sem_wait(philo->semaphore->write);
	sem_post(philo->semaphore->global_died);
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
	sem_post(philo->semaphore->write);
	return (NULL);
}
