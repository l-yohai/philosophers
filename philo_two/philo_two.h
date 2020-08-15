/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yohlee <yohlee@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/15 00:52:10 by yohlee            #+#    #+#             */
/*   Updated: 2020/08/16 05:23:46 by yohlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <sys/time.h>

# define TAKE_FORK 0
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
# define MSG_ERROR_ARGC "Error: invalid arg. Check argc is 5 or 6.\n"
# define MSG_ERROR_ARGV "Error: invalid arg. Check argv is positive numeric.\n"
# define MSG_ERROR_PHILO "Error: num of philosophers should be at least 2.\n"
# define MSG_ERROR_MALLOC "Error: memory allocation.\n"
# define MSG_ERROR_SEM "Error: semaphore.\n"

int					g_philo_died;
int					g_philos_satiated;

typedef struct		s_semaphore
{
	sem_t			*fork;
	sem_t			*write;
	sem_t			*global_died;
	sem_t			*global_satiated;
}					t_semaphore;

typedef struct		s_data
{
	int				num_of_philosophers;
	int				num_of_times_each_philosopher_must_eat;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		time_of_start;
}					t_data;

typedef struct		s_philo
{
	pthread_t		thread;
	int				id;
	int				next_id;
	int				count_eat;
	uint64_t		time_of_last_eat;
	sem_t			*last_eat;
	t_data			*data;
	t_semaphore		*semaphore;
}					t_philo;

/*
**		cycle.c
*/

void				*cycle(void *philo);
int					eat(t_philo *philo);
int					take_forks(t_philo *philo);
void				*monitor_eat(void *philo);
void				*monitor_death(void *philo);

/*
**		init.c
*/

int					init_data(char **argv, t_data *data);
int					init_semaphore(t_philo **philo, t_semaphore *sem, int num);
int					unlink_semaphores(int num);

/*
**		libft.c
*/

size_t				ft_strlen(const char *s);
void				ft_putchar(char c);
void				ft_putnbr(unsigned long n);
unsigned long		ft_atoul(const char *s);
char				*ft_ultoa(unsigned long n);
char				*ft_strjoin(char *s1, char *s2);
/*
**		print.c
*/

void				print_msg(t_philo *philo, int index, uint64_t time);
int					print_status(t_philo *philo, int index);
void				*print_exit(t_philo *philo, int index, uint64_t time);

/*
**		utils.c
*/

uint64_t			get_time(void);
int					check_arguments(char **argv);
int					exit_error(char *s);
void				clean(t_philo *philo);

/*
**		ft_strjoin.c
*/

char				*ft_strdup(char *s);
char				*ft_strjoin(char *s1, char *s2);

#endif
