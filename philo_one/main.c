/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 14:16:34 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/06 20:20:31 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"
#include <stdio.h>

# define TYPE_EAT 	0
# define TYPE_SLEEP 1
# define TYPE_FORK 	2
# define TYPE_THINK	3
# define TYPE_DIED 	4
# define TYPE_OVER 	5

int	ft_atoi(const char *nptr)
{
	int	i;
	int	is_n;
	int	num;

	i = 0;
	is_n = 1;
	num = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			is_n *= (-1);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + nptr[i] - '0';
		i++;
	}
	return (num * is_n);
}

static int init_mutexes(t_state *state)
{
	int	i;

	pthread_mutex_init(&state->write_m, NULL);
	pthread_mutex_init(&state->somebody_dead_m, NULL);
	pthread_mutex_lock(&state->somebody_dead_m);
	if (!(state->forks_m =
		(pthread_mutex_t*)malloc(sizeof(*(state->forks_m)) * state->nums)))
		return (1);
	i = 0;
	while (i < state->nums)
		pthread_mutex_init(&state->forks_m[i++], NULL);
	return (0);
}

static void init_philos(t_state *state)
{
	int	i;

	i = 0;
	while (i < state->nums)
	{
		state->philos[i].is_eating = 0;
		state->philos[i].position = i;
		state->philos[i].lfork = i;
		state->philos[i].rfork = (i + 1) % state->nums;
		state->philos[i].eat_count = 0;
		state->philos[i].state = state;
		pthread_mutex_init(&state->philos[i].mutex, NULL);
		pthread_mutex_init(&state->philos[i].eat_m, NULL);
		pthread_mutex_lock(&state->philos[i].eat_m);
		i++;
	}
}

int	init(t_state *state, int argc, char **argv)
{
	state->nums = ft_atoi(argv[1]);
	state->time_to_die = ft_atoi(argv[2]);
	state->time_to_eat = ft_atoi(argv[3]);
	state->time_to_sleep = ft_atoi(argv[4]);
	state->nums_to_eat = (argc == 6) ? ft_atoi(argv[5]) : 0;
	if (state->nums < 2 || state->nums > 200 || state->time_to_die < 60 ||\
		state->time_to_eat < 60 || state->time_to_sleep < 60 || state->nums_to_eat < 0)
		return (-1);
	state->forks_m = NULL;
	state->philos = NULL;
	if (!(state->philos = (t_philo *)malloc(sizeof(*(state->philos)) * state->nums)))
		return (-1);
	init_philos(state);
	return (init_mutexes(state));
}

void display_message(t_philo *philo, int type)
{
	static int	done = 0;

	pthread_mutex_lock(&philo->state->write_m);
	if (!done)
	{
		ft_putnbr_fd(get_time() - philo->state->start, 1);
		write(1, "\t", 1);
		if (type != TYPE_OVER)
			ft_putnbr_fd(philo->position + 1, 1);
		if (type >= TYPE_DIED)
			done = 1;
		write(1, get_message(type), ft_strlen(get_message(type)));
	}
	pthread_mutex_unlock(&philo->state->write_m);
}

static void *monitor_count(void *state_v)
{
	t_state *state;
	int		i;
	int		total;

	state = (t_state*)state_v;
	total = 0;
	while (total < state->nums_to_eat)
	{
		i = 0;
		while (i < state->nums)
			pthread_mutex_lock(&state->philos[i++].eat_m);
		total++;
	}
	display_message(&state->philos[0], TYPE_OVER);
	pthread_mutex_unlock(&state->somebody_dead_m);
	return ((void*)0);
}

static int start_threads(t_state *state)
{
	int			i;
	pthread_t	tid;
	void		*philo;

	state->start = get_time();
	if (state->nums_to_eat > 0)
	{
		if (pthread_create(&tid, NULL, &monitor_count, (void*)state) != 0)
			return (1);
		pthread_detach(tid);
	}
	i = 0;
	while (i < state->nums)
	{
		philo = (void*)(&state->philos[i]);
		if (pthread_create(&tid, NULL, &routine, philo) != 0)
			return (1);
		pthread_detach(tid);
		usleep(100);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_state	state;

	if (init(&state, argc, argv))
		return (clear_state(&state) && exit_error("error: fatal\n"));
}