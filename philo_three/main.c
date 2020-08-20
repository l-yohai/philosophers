/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 00:52:01 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/20 18:22:57 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	create_process(t_data *data, t_semaphore *sem, pid_t *pid)
{
	t_philo	philo;
	pid_t	pid2;
	int		i;

	i = 0;
	while (i < data->num_of_philosophers)
	{
		philo.id = i;
		philo.data = data;
		philo.semaphore = sem;
		if (i == 0 && data->num_of_times_each_philosopher_must_eat > 0)
			pid2 = fork();
		if (i == 0 &&\
				data->num_of_times_each_philosopher_must_eat > 0 && pid2 == 0)
			monitor_eat(&philo);
		if ((pid[i] = fork()) == 0)
		{
			free(pid);
			cycle(&philo);
		}
		usleep(30);
		i++;
	}
	clean(pid, pid2, data, sem);
}

void	run(t_data *data, t_semaphore *sem)
{
	pid_t		*pid;

	if (!(pid = (pid_t *)malloc(sizeof(pid_t) * (data->num_of_philosophers))))
		exit_error(MSG_ERROR_MALLOC);
	data->time_of_start = get_time();
	create_process(data, sem, pid);
}

int		main(int argc, char **argv)
{
	t_data		data;
	t_semaphore	sem;

	if (argc != 5 && argc != 6)
		return (exit_error(MSG_ERROR_ARGC));
	if (init_data(argv, &data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_semaphore(&sem, data.num_of_philosophers) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	run(&data, &sem);
	return (EXIT_SUCCESS);
}
