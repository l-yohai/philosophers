/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 00:52:01 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/15 05:17:18 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

# define TOOK_FORK 0
# define IS_EATING 1
# define IS_SLEEPING 2
# define IS_THINKING 3
# define HAS_DIED 4
# define ARE_SATIATED 5

# define MSG_TAKE_FORK " has taken a fork\n"
# define MSG_EAT " is eating\n"
# define MSG_SLEEP " is sleeping\n"
# define MSG_THINK " is thinking\n"
# define MSG_DIE " died\n"
# define MSG_FINISH "All philosophers ate enough\n"

int g_philo_died;
int g_philos_satiated;

typedef struct		s_mutex
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	write;
	pthread_mutex_t global_died;
	pthread_mutex_t global_satiated;
}					t_mutex;

typedef struct		s_data
{
	int				number_of_philosophers;
	int				number_of_times_each_philosopher_must_eat;
	unsigned long	time_of_start;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
}					t_data;

typedef struct		s_philo
{
	pthread_t		thread_id;
	int				id;
	int				next_id;
	int				count_eat;
	unsigned long	time_of_last_eat;
	pthread_mutex_t	last_eat;
	t_data			*data;
	t_mutex			*mutex;
}					t_philo;

unsigned long	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

size_t			ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		++i;
	return (i);
}

unsigned long	ft_atoul(const char *str)
{
	size_t			i;
	unsigned long	nb;

	i = 0;
	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 - (str[i] - '0');
		++i;
	}
	return (-nb);
}

void			ft_putnbr(unsigned long n)
{
	char c;

	if (n > 9)
		ft_putnbr(n / 10);
	c = n % 10 + '0';
	write(1, &c, 1);
}

int		exit_error(char *s)
{
	write(2, s, ft_strlen(s));
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

int		valid_arguments(char *av[])
{
	int i;
	int j;

	i = 0;
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (0);
		}
	}
	return (1);
}

void	print_msg(t_philo *philo, int index, unsigned long time)
{
	ft_putnbr(time - philo->data->time_of_start);
	write(1, "\t", 1);
	ft_putnbr(philo->id + 1);
	if (index == TOOK_FORK)
		write(1, MSG_TAKE_FORK, ft_strlen(MSG_TAKE_FORK));
	else if (index == IS_EATING)
		write(1, MSG_EAT, ft_strlen(MSG_EAT));
	else if (index == IS_SLEEPING)
		write(1, MSG_SLEEP, ft_strlen(MSG_SLEEP));
	else if (index == IS_THINKING)
		write(1, MSG_THINK, ft_strlen(MSG_THINK));
}

void	*print_exit(t_philo *philo, int index, unsigned long timestamp)
{
	pthread_mutex_lock(&philo->mutex->write);
	pthread_mutex_unlock(&philo->mutex->global_died);
	if (index == HAS_DIED)
	{
		ft_putnbr(timestamp - philo->data->time_of_start);
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
		pthread_mutex_lock(&mutex->global_satiated);
		if (g_philos_satiated == ((t_philo *)philo)->data->number_of_philosophers)
		{
			pthread_mutex_unlock(&mutex->global_satiated);
			g_philo_died = 1;
			return (print_exit(philo, ARE_SATIATED, 1));
		}
		pthread_mutex_unlock(&mutex->global_died);
		pthread_mutex_unlock(&mutex->global_satiated);
		usleep(10);
	}
}

void	*monitor_death(void *philo)
{
	t_philo			*p;
	unsigned long	timestamp;

	p = philo;
	while (1)
	{
		pthread_mutex_lock(&p->last_eat);
		if ((timestamp = get_time()) - p->time_of_last_eat >
			p->data->time_to_die)
		{
			pthread_mutex_unlock(&p->last_eat);
			pthread_mutex_lock(&p->mutex->global_died);
			if (!g_philo_died)
			{
				g_philo_died = 1;
				return (print_exit(p, HAS_DIED, timestamp));
			}
			pthread_mutex_unlock(&p->mutex->global_died);
			return (NULL);
		}
		pthread_mutex_unlock(&p->last_eat);
		usleep(10);
	}
}

int		print_status(t_philo *philo, int index)
{
	pthread_mutex_lock(&philo->mutex->global_died);
	if (g_philo_died)
	{
		pthread_mutex_unlock(&philo->mutex->global_died);
		return (0);
	}
	pthread_mutex_lock(&philo->mutex->write);
	pthread_mutex_unlock(&philo->mutex->global_died);
	print_msg(philo, index, get_time());
	pthread_mutex_unlock(&philo->mutex->write);
	return (1);
}

int		take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex->fork[philo->id]);
	if (!print_status(philo, TOOK_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
		return (0);
	}
	pthread_mutex_lock(&philo->mutex->fork[philo->next_id]);
	if (!print_status(philo, TOOK_FORK))
	{
		pthread_mutex_unlock(&philo->mutex->fork[philo->id]);
		pthread_mutex_unlock(&philo->mutex->fork[philo->next_id]);
		return (0);
	}
	return (1);
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
	if (++philo->count_eat == philo->data->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&philo->mutex->global_satiated);
		++g_philos_satiated;
		pthread_mutex_unlock(&philo->mutex->global_satiated);
	}
	return (1);
}

void	*cycle(void *ph)
{
	t_philo		*philo;
	pthread_t	thread_id;

	philo = ph;
	philo->next_id = (philo->id == philo->data->number_of_philosophers - 1) ?
							0 : philo->id + 1;
	philo->count_eat = 0;
	philo->time_of_last_eat = philo->data->time_of_start;
	pthread_create(&thread_id, NULL, monitor_death, philo);
	while (1)
	{
		if (!eat(philo))
			break ;
		if (!print_status(philo, IS_SLEEPING))
			break ;
		usleep(philo->data->time_to_sleep);
		if (!print_status(philo, IS_THINKING))
			break ;
	}
	pthread_join(thread_id, NULL);
	return (NULL);
}

int		parse(char *av[], t_data *data)
{
	if (!valid_arguments(av))
		return (exit_error("Error: invalid argument. Check argv is positive numeric.\n"));
	data->number_of_philosophers = ft_atoul(av[1]);
	if (data->number_of_philosophers < 2)
		return (exit_error("Error: number of philosophers should be at least 2.\n"));
	data->number_of_times_each_philosopher_must_eat = (av[5]) ? ft_atoul(av[5]) : -1;
	data->time_to_die = ft_atoul(av[2]);
	data->time_to_eat = ft_atoul(av[3]) * 1000;
	data->time_to_sleep = ft_atoul(av[4]) * 1000;
	return (EXIT_SUCCESS);
}

void	cleanup(t_philo *philo)
{
	int i;

	i = -1;
	while (++i < philo->data->number_of_philosophers)
	{
		pthread_mutex_destroy(&philo->mutex->fork[i]);
		pthread_mutex_destroy(&philo[i].last_eat);
	}
	pthread_mutex_destroy(&philo->mutex->write);
	pthread_mutex_destroy(&philo->mutex->global_died);
	pthread_mutex_destroy(&philo->mutex->global_satiated);
	free(philo->mutex->fork);
	free(philo);
}

int		init(t_philo **philo, t_mutex *mutex, int number_of_philosophers)
{
	int			i;
	static int	ret;

	mutex->fork = NULL;
	*philo = NULL;
	if (!(mutex->fork = malloc(sizeof(*mutex->fork) * number_of_philosophers)) ||
		!(*philo = malloc(sizeof(**philo) * number_of_philosophers)))
		return (exit_error("Error: memory allocation.\n"));
	i = -1;
	while (++i < number_of_philosophers)
	{
		ret += pthread_mutex_init(&mutex->fork[i], NULL);
		ret += pthread_mutex_init(&(*philo)[i].last_eat, NULL);
	}
	ret += pthread_mutex_init(&mutex->write, NULL);
	ret += pthread_mutex_init(&mutex->global_died, NULL);
	ret += pthread_mutex_init(&mutex->global_satiated, NULL);
	(ret != 0) ? cleanup(*philo) : 0;
	if (ret != 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

void	run(t_philo *philo, t_data *data, t_mutex *mutex)
{
	int			i;
	pthread_t	thread_id;

	i = -1;
	while (++i < data->number_of_philosophers)
	{
		philo[i].id = i;
		philo[i].data = data;
		philo[i].mutex = mutex;
	}
	data->time_of_start = get_time();
	i = -1;
	while (++i < data->number_of_philosophers)
	{
		pthread_create(&philo[i].thread_id, NULL, cycle, &philo[i]);
		(i == 0 && data->number_of_times_each_philosopher_must_eat > 0) ?
			pthread_create(&thread_id, NULL, monitor_eat, &philo[i]) : 0;
		usleep(50);
	}
	(data->number_of_times_each_philosopher_must_eat > 0) ? pthread_join(thread_id, NULL) : 0;
	i = -1;
	while (++i < data->number_of_philosophers)
		pthread_join(philo[i].thread_id, NULL);
}

int		main(int ac, char **av)
{
	t_philo		*philo;
	t_data		data;
	t_mutex		mutex;

	if ((ac != 5 && ac != 6))
		return (exit_error("Error: invalid argument. Check argc is 5 or 6.\n"));
	philo = NULL;
	if (parse(av, &data) == EXIT_FAILURE ||\
			init(&philo, &mutex, data.number_of_philosophers) == EXIT_SUCCESS)
		return (EXIT_FAILURE);
	run(philo, &data, &mutex);
	cleanup(philo);
	return (EXIT_SUCCESS);
}