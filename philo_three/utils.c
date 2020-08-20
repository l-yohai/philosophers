/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 22:03:16 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/18 09:10:38 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

uint64_t	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int			check_arguments(char **argv)
{
	int i;
	int j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void		clean(pid_t *pid, pid_t pid2, t_data *data, t_semaphore *sem)
{
	int	i;

	sem_wait(sem->simulation_end);
	i = 0;
	while (i < data->num_of_philosophers)
	{
		kill(pid[i], SIGKILL);
		i++;
	}
	if (data->num_of_times_each_philosopher_must_eat > 0)
		kill(pid2, SIGKILL);
	unlink_semaphores(data->num_of_philosophers);
	free(pid);
}

int			exit_error(char *s)
{
	write(2, s, ft_strlen(s));
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}
