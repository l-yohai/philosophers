/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 00:52:01 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/15 04:30:57 by yohlee           ###   ########.fr       */
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

# define HAS_DIED 0
# define ARE_SATIATED 1

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
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	uint64_t		time_of_start;
}					t_data;

typedef struct		s_philo
{
	pthread_t		tid;
	int				id;
	int				next_id;
	int				count_eat;
	uint64_t		time_of_last_eat;
	pthread_mutex_t	last_eat;
	t_data			*data;
	t_mutex			*mutex;
}					t_philo;

void	ft_putchar(const char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	if (n >= 0)
	{
		if (n > 9)
			ft_putnbr(n / 10);
		ft_putchar(n % 10 + '0');
	}
	else
	{
		ft_putchar('-');
		if (n < -9)
			ft_putnbr(n / (-10));
		ft_putchar((n % 10 * -1) + '0');
	}
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_atoul(char *s)
{
	unsigned long	i;
	unsigned long	num;

	i = 0;
	num = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		num = num * 10 + s[i] - '0';
		i++;
	}
	return (num);
}

int	exit_error(char *s)
{
	write(1, s, ft_strlen(s));
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

void	clean(t_philo *philo)
{
	int i;

	i = 0;
	while (i < philo->data->number_of_philosophers)
	{
		pthread_mutex_destroy(&philo->mutex->fork[i]);
		pthread_mutex_destroy(&philo[i].last_eat);
		i++;
	}
	pthread_mutex_destroy(&philo->mutex->write);
	pthread_mutex_destroy(&philo->mutex->global_died);
	pthread_mutex_destroy(&philo->mutex->global_satiated);
	free(philo->mutex->fork);
	free(philo);
}

int	check_arg(char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (-1);
			j++;
		}
		i++;
	}
	return (1);
}

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int		print_status(t_philo *philo, int index)
{
	static char	*msg[4] = {" has taken a fork\n", " is eating\n",
							" is sleeping\n", " is thinking\n"};
	static int	len[4] = {18, 11, 13, 13};

	pthread_mutex_lock(&philo->mutex->global_died);
	if (g_philo_died)
	{
		pthread_mutex_unlock(&philo->mutex->global_died);
		return (0);
	}
	pthread_mutex_lock(&philo->mutex->write);
	pthread_mutex_unlock(&philo->mutex->global_died);
	ft_putnbr(get_time() - philo->data->time_of_start);
	write(1, "\t", 1);
	ft_putnbr(philo->id + 1);
	write(1, msg[index], len[index]);
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

void	*print_exit(t_philo *philo, int index, unsigned long timestamp)
{
	static char *msg[2] = {" has died\n", "All philosophers ate enough\n"};
	static int	len[2] = {10, 28};

	pthread_mutex_lock(&philo->mutex->write);
	pthread_mutex_unlock(&philo->mutex->global_died);
	if (index == HAS_DIED)
	{
		ft_putnbr(timestamp - philo->data->time_of_start);
		write(1, "\t", 1);
		ft_putnbr(philo->id + 1);
	}
	write(1, msg[index], len[index]);
	pthread_mutex_unlock(&philo->mutex->write);
	return (NULL);
}


void	*monitor_eat(void *ph)
{
	t_mutex *mutex;

	mutex = ((t_philo *)ph)->mutex;
	while (1)
	{
		pthread_mutex_lock(&mutex->global_died);
		if (g_philo_died)
		{
			pthread_mutex_unlock(&mutex->global_died);
			return (NULL);
		}
		pthread_mutex_lock(&mutex->global_satiated);
		if (g_philos_satiated == ((t_philo *)ph)->data->number_of_philosophers)
		{
			pthread_mutex_unlock(&mutex->global_satiated);
			g_philo_died = 1;
			return (print_exit(ph, ARE_SATIATED, 1));
		}
		pthread_mutex_unlock(&mutex->global_died);
		pthread_mutex_unlock(&mutex->global_satiated);
		usleep(10);
	}
}

/*
** monitoring thread created by each philosopher to monitor it's own health
*/

void	*monitor_death(void *ph)
{
	t_philo			*philo;
	unsigned long	timestamp;

	philo = ph;
	while (1)
	{
		pthread_mutex_lock(&philo->last_eat);
		if ((timestamp = get_time()) - philo->time_of_last_eat >
			philo->data->time_to_die)
		{
			pthread_mutex_unlock(&philo->last_eat);
			pthread_mutex_lock(&philo->mutex->global_died);
			if (!g_philo_died)
			{
				g_philo_died = 1;
				return (print_exit(philo, HAS_DIED, timestamp));
			}
			pthread_mutex_unlock(&philo->mutex->global_died);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->last_eat);
		usleep(10);
	}
}

void	*cycle(void *philo)
{
	t_philo	*p;
	pthread_t	tid;

	p = philo;
	p->next_id = (p->id == p->data->number_of_philosophers - 1) ? 0 : p->id + 1;
	p->time_of_last_eat = p->data->time_of_start;
	pthread_create(&tid, NULL, monitor_death, p);
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
	pthread_join(tid, NULL);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;
	t_data	data;
	t_mutex	mutex;
	static int	ret;
	int	i;

	if (argc != 5 && argc != 6)
		return (exit_error("Error: invalid argument. Check argc is 5 or 6.\n"));
	philo = NULL;
	if (!check_arg(argv))
		return (exit_error("Error: invalid argument. Check argv is positive numeric.\n"));
	data.number_of_philosophers = ft_atoul(argv[1]);
	if (data.number_of_philosophers < 2)
		return (exit_error("Error: number of philosophers should be at least 2.\n"));
	data.time_to_die = ft_atoul(argv[2]);
	data.time_to_eat = ft_atoul(argv[3]);
	data.time_to_sleep = ft_atoul(argv[4]);
	data.number_of_times_each_philosopher_must_eat = (argc == 6) ? ft_atoul(argv[5]) : -1;
	
	
	if (!(mutex.fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (data.number_of_philosophers))))
		return (exit_error("Error: memory allocation.\n"));
	if (!(philo = (t_philo *)malloc(sizeof(t_philo) * (data.number_of_philosophers))))
		return (exit_error("Error: memory allocation.\n"));
	
	i = 0;
	while (i < data.number_of_philosophers)
	{
		ret += pthread_mutex_init(&mutex.fork[i], NULL);
		ret += pthread_mutex_init(&philo[i].last_eat, NULL);
		i++;
	}
	ret += pthread_mutex_init(&mutex.write, NULL);
	ret += pthread_mutex_init(&mutex.global_died, NULL);
	ret += pthread_mutex_init(&mutex.global_satiated, NULL);
	
	// (ret != 0) ? clean(philo) : 0;
	// if (ret != 0)
	// 	return (0);
	
	pthread_t	tid;

	i = 0;
	while (i < data.number_of_philosophers)
	{
		philo[i].id = i;
		philo[i].data = &data;
		philo[i].mutex = &mutex;
	}
	data.time_of_start = get_time();
	
	i = 0;
	while (i < data.number_of_philosophers)
	{
		pthread_create(&philo[i].tid, NULL, cycle, &philo[i]);
		(i == 0 && data.number_of_times_each_philosopher_must_eat > 0) ?
			pthread_create(&tid, NULL, monitor_eat, &philo[i]) : 0;
		usleep(50);
		i++;
	}
	(data.number_of_times_each_philosopher_must_eat > 0) ? pthread_join(tid, NULL) : 0;
	i = 0;
	while (i < data.number_of_philosophers)
	{
		pthread_join(philo[i].tid, NULL);
		i++;
	}
}