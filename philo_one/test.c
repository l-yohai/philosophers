/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 14:16:34 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/08 16:28:36 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

#include <stdio.h>

typedef struct		s_data
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;

	int				index;
	int				dead;
	int				print_dead;

	pthread_t		*thread;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	inc_mutex;
	pthread_mutex_t	print_mutex;

	struct timeval	begin;
	struct timeval	*last_eat;

}					t_data;

int					ft_atoi(const char *nptr);
void				ft_putstr(char *s);
void				ft_putchar(char c);
void				ft_putnbr_unsigned_long(long nb);
int		init_data(int argc, char **argv, t_data *data);
void			print_struct(t_data *data);
void			free_struct(t_data *data);
double			get_time(struct timeval begin, struct timeval end);
int				declare_fork(t_data *data, int index);
int				declare_eat(t_data *data, int index);
int				declare_sleep(t_data *data, int index);
int				declare_think(t_data *data, int index);
int				declare_died(t_data *data, int index, struct timeval now);
int				check_dead(t_data *data);
void			unlock_mutexs(t_data *data);

int		ft_atoi(const char *nptr)
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

void	ft_putstr(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	write(1, s, i);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr_unsigned_long(long nb)
{
	if (nb > 9)
	{
		ft_putnbr_unsigned_long(nb / 10);
		ft_putnbr_unsigned_long(nb % 10);
	}
	else
		ft_putchar(nb + '0');
}

double	get_time(struct timeval begin, struct timeval end)
{
	double elapsed;

	elapsed = ((end.tv_sec - begin.tv_sec) * 1000000) +
				((end.tv_usec - begin.tv_usec));
	return (elapsed / 1000);
}

void	unlock_mutexs(t_data *data)
{
	int i;

	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_unlock(&data->mutex[i++]);
	pthread_mutex_unlock(&data->inc_mutex);
	pthread_mutex_unlock(&data->print_mutex);
}

int	declare_fork(t_data *data, int index)
{
	struct timeval end;

	if (data->dead || check_dead(data))
		return (1);
	gettimeofday(&end, NULL);
	pthread_mutex_lock(&data->print_mutex);
	if (data->dead || check_dead(data))
		return (1);
	ft_putnbr_unsigned_long(get_time(data->begin, end));
	ft_putchar(' ');
	ft_putnbr_unsigned_long((long)index);
	ft_putstr(" has taken a fork\n");
	pthread_mutex_unlock(&data->print_mutex);
	return (0);
}

int	declare_eat(t_data *data, int index)
{
	struct timeval end;

	if (data->dead || check_dead(data))
		return (1);
	gettimeofday(&end, NULL);
	pthread_mutex_lock(&data->print_mutex);
	if (data->dead || check_dead(data))
		return (1);
	ft_putnbr_unsigned_long(get_time(data->begin, end));
	ft_putchar(' ');
	ft_putnbr_unsigned_long((long)index);
	ft_putstr(" is eating\n");
	pthread_mutex_unlock(&data->print_mutex);
	return (0);
}

int	declare_sleep(t_data *data, int index)
{
	struct timeval	end;

	if (data->dead || check_dead(data))
		return (1);
	gettimeofday(&end, NULL);
	pthread_mutex_lock(&data->print_mutex);
	if (data->dead || check_dead(data))
		return (1);
	ft_putnbr_unsigned_long(get_time(data->begin, end));
	ft_putchar(' ');
	ft_putnbr_unsigned_long((long)index);
	ft_putstr(" is sleeping\n");
	pthread_mutex_unlock(&data->print_mutex);
	return (0);
}

int	declare_think(t_data *data, int index)
{
	struct timeval end;

	if (data->dead || check_dead(data))
		return (1);
	gettimeofday(&end, NULL);
	pthread_mutex_lock(&data->print_mutex);
	if (data->dead || check_dead(data))
		return (1);
	ft_putnbr_unsigned_long(get_time(data->begin, end));
	ft_putchar(' ');
	ft_putnbr_unsigned_long((long)index);
	ft_putstr(" is thinking\n");
	pthread_mutex_unlock(&data->print_mutex);
	return (0);
}

int	declare_died(t_data *data, int index, struct timeval now)
{
	pthread_mutex_lock(&data->print_mutex);
	if (data->print_dead)
	{
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	ft_putnbr_unsigned_long(get_time(data->begin, now));
	ft_putchar(' ');
	ft_putnbr_unsigned_long((long)index);
	ft_putstr(" died\n");
	pthread_mutex_unlock(&data->print_mutex);
	data->dead = 1;
	data->print_dead = 1;
	return (0);
}

int		check_dead(t_data *data)
{
	struct timeval	now;
	int				i;
	int				dead;

	dead = 0;
	i = 0;
	gettimeofday(&now, NULL);
	while (i < data->number_of_philosophers)
	{
		if (((int)get_time(data->last_eat[i++], now)) > data->time_to_die)
		{
			dead = i;
			break ;
		}
	}
	if (!dead || data->dead)
		return (0);
	declare_died(data, dead, now);
	unlock_mutexs(data);
	return (1);
}

int		init_thread(t_data *data)
{
	int i;

	i = 0;
	if (!(data->last_eat = malloc(sizeof(struct timeval) *
		data->number_of_philosophers)))
		return (-1);
	if (!(data->thread = malloc(sizeof(pthread_t) * data->number_of_philosophers)))
		return (-1);
	if (data->number_of_philosophers > 1)
	{
		if (!(data->mutex = malloc(sizeof(pthread_mutex_t) *
			data->number_of_philosophers)))
			return (-1);
	}
	else
	{
		if (!(data->mutex = malloc(sizeof(pthread_mutex_t) * 2)))
			return (-1);
	}
	while (i < data->number_of_philosophers)
		pthread_mutex_init(&data->mutex[i++], NULL);
	pthread_mutex_init(&data->inc_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	return (1);
}

void	destroy_mutexs(t_data *data)
{
	int i;

	i = 0;
	while (i < data->number_of_philosophers)
		pthread_mutex_destroy(&data->mutex[i++]);
}

void	free_struct(t_data *data)
{
	destroy_mutexs(data);
	free(data->thread);
	free(data->mutex);
	free(data->last_eat);
	free(data);
}

void	print_struct(t_data *data)
{
	printf("number_of_philosophers : %d\n", data->number_of_philosophers);
	printf("time_to_die : %d\n", data->time_to_die);
	printf("time_to_eat : %d\n", data->time_to_eat);
	printf("time_to_sleep : %d\n", data->time_to_sleep);
	printf("number_of_times_each_philosopher_must_eat : %d\n", data->number_of_times_each_philosopher_must_eat);
}

int		take_fork(t_data *data, int i)
{
	if (data->dead || check_dead(data))
		return (1);
	pthread_mutex_lock(&data->mutex[i]);
	if (i < data->number_of_philosophers - 1 || data->number_of_philosophers == 1)
		pthread_mutex_lock(&data->mutex[i + 1]);
	else
		pthread_mutex_lock(&data->mutex[0]);
	if (data->dead || check_dead(data))
		return (1);
	declare_fork(data, i + 1);
	declare_fork(data, i + 1);
	return (0);
}

void	put_fork(t_data *data, int i)
{
	pthread_mutex_unlock(&data->mutex[i]);
	if (i < data->number_of_philosophers - 1 || data->number_of_philosophers == 1)
		pthread_mutex_unlock(&data->mutex[i + 1]);
	else
		pthread_mutex_unlock(&data->mutex[0]);
}

void	*philo_thread(void *arg)
{
	t_data			*data;
	int				i;

	data = (t_data *)arg;
	if (data->dead || check_dead(data))
		return (arg);
	pthread_mutex_lock(&data->inc_mutex);
	i = ++data->index;
	pthread_mutex_unlock(&data->inc_mutex);
	if (take_fork(data, i))
		return (arg);
	if (declare_eat(data, i + 1))
		return (arg);
	gettimeofday(&data->last_eat[i], NULL);
	usleep(data->time_to_eat * 1000);
	gettimeofday(&data->last_eat[i], NULL);
	put_fork(data, i);
	if (declare_sleep(data, i + 1))
		return (arg);
	usleep(data->time_to_sleep * 1000);
	if (declare_think(data, i + 1))
		return (arg);
	return (NULL);
}

int		init_thread(t_data *data)
{
	int	i;

	if (!(data->thread = malloc(sizeof(pthread_t) * data->number_of_philosophers)))
		return (-1);
	if (!(data->mutex = malloc(sizeof(pthread_mutex_t) *\
			(data->number_of_philosophers > 1) ?\
				data->number_of_philosophers : 2)))
		return (-1);
	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_init(&data->mutex[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->inc_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	return (1);
}

int		init_data(int argc, char **argv, t_data *data)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->number_of_times_each_philosopher_must_eat =\
		(argc == 6) ? ft_atoi(argv[5]) : 0;
	if (data->number_of_times_each_philosopher_must_eat < 1 ||\
		data->number_of_philosophers < 1 || data->time_to_eat < 1 ||\
		data->time_to_die < 1 || data->time_to_sleep < 1)
		return (-1);
	data->index = 0;
	data->dead = 0;
	data->print_dead = 0;
	data->thread = NULL;
	data->mutex = NULL;
	if (!init_thread(data))
		return (-1);
	return (1);
}

int		main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (argc != 5 && argc != 6)
		return (-1);
	if (!init_data(argc, argv, &data))
		return (-1);

	gettimeofday(&data.begin, NULL);
	while (data.number_of_times_each_philosopher_must_eat-- != 0 && !data.dead)
	{
		i = 0;
		data.index = -1;
		while (i < data.number_of_philosophers)
			pthread_create(&data.thread[i++], NULL, philo_thread, &data);
		i--;
		while (i >= 0)
			pthread_join(data.thread[i--], NULL);
	}
	// free_struct(&data);
	return (0);
}