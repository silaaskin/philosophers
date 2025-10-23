/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: silaaskin <silaaskin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:49:43 by silaaskin         #+#    #+#             */
/*   Updated: 2025/10/23 20:03:11 by silaaskin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H_
# define PHILO_H_

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct  s_rules
{
    int                 num_philos;
    int                 time_to_die;
    int                 time_to_eat;
    int                 time_to_sleep;
    int                 num_must_eat;
    pthread_mutex_t     *forks;
    pthread_mutex_t     print_mutex;
    long                start_time;
    bool                simulation_stop;
    pthread_mutex_t     sim_end_mutex;

}   t_rules;

typedef struct  s_philo
{
    int            id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    int            meals_eaten;
    long           last_meal_time;
    t_rules        *rules;
    pthread_t       thread;
    pthread_mutex_t meal_mutex;
    

}   t_philo;

long    get_time_in_ms(void);
void    init_philos(t_philo *philos, t_rules *rules);
void    init_forks(t_rules *rules);
int     check_arg(int argc, char **argv);
void    init_structers(int argc, char **argv, t_rules *rules);
void    init_thread(t_philo *philo);
void    print_action(t_philo *philo, char *output, int flag);
void    think(t_philo *philo);
void    *one_philo(t_philo *philo);
void    pick_forks(t_philo  *philo);
void    drop_forks(t_philo *philo);
void    eat(t_philo *philo);
void    my_sleep(t_philo   *philo);
void    set_simulation_stopped(t_philo *philo);
void    check_philo_death(t_philo *philo);
void    check_all_eaten(t_philo *philo);
void    *philo_routine(void  *arg);
int     is_simulation_stopped(t_rules *rules);
void    *monitor_routine(void *arg);
long	ft_atoi(const char *str);
void    ft_usleep(long time_ms);

#endif
 