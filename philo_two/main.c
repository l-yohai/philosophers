/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 00:52:01 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/16 02:54:34 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	run(t_philo *philo, t_data *data)
{
	int			i;
	pthread_t	thread;

	data->time_of_start = get_time();
	i = 0;
	while (i < data->num_of_philosophers)
	{
		pthread_create(&philo[i].thread, NULL, cycle, &philo[i]);
		if (i == 0 && data->num_of_times_each_philosopher_must_eat > 0)
			pthread_create(&thread, NULL, monitor_eat, &philo[i]);
		usleep(50);
		i++;
	}
	if (data->num_of_times_each_philosopher_must_eat > 0)
		pthread_join(thread, NULL);
	i = 0;
	while (i < data->num_of_philosophers)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

int		main(int argc, char **argv)
{
	t_philo		*philo;
	t_data		data;
	t_semaphore	sem;
	int			i;

	if ((argc != 5 && argc != 6))
		return (exit_error(MSG_ERROR_ARGC));
	philo = 0;
	if (init_data(argv, &data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (init_thread(\
			&philo, &sem, data.num_of_philosophers) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = 0;
	while (i < data.num_of_philosophers)
	{
		philo[i].id = i;
		philo[i].data = &data;
		philo[i].semaphore = &sem;
		i++;
	}
	run(philo, &data);
	clean(philo);
	return (EXIT_SUCCESS);
}
