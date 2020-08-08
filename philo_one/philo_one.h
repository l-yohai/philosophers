/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 05:11:01 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/08 16:17:47 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>


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

/*
**					utils.c
*/

int					ft_atoi(const char *nptr);
void				ft_putstr(char *s);
void				ft_putchar(char c);
void				ft_putnbr_unsigned_long(long nb);

int				get_args(int ac, char **av, t_data *data);
t_data			*create_struct_pointer();
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

#endif