 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saskin <saskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:38:26 by saskin            #+#    #+#             */
/*   Updated: 2025/10/11 19:16:46 by saskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

void    print_action(t_philo *philo, char *output, int flag)
{
    long time;
    int stopped;
    stopped = is_simulation_stopped(philo->rules);//yerini değiştirdim bi etkisi olmadı
    pthread_mutex_lock(&philo->rules->print_mutex);
    time = get_time_in_ms() - philo->rules->start_time;
    if (!stopped && flag == 1)
        printf("%d %s\n",philo->meals_eaten, output);
    else if (!stopped)
        printf("%ld %d %s\n", time, philo->id, output);
    pthread_mutex_unlock(&philo->rules->print_mutex);
    
}
void    *one_philo(t_philo *philo)
{
    print_action(philo, "is thinking", 0);
    pthread_mutex_lock(philo->left_fork);
    print_action(philo,"has taken a fork", 0);
    usleep(philo->rules->time_to_die * 1000);
    pthread_mutex_unlock(philo->left_fork);
    return NULL;
}
void    pick_forks(t_philo  *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->left_fork);
        print_action(philo, "has taken a fork", 0);
        pthread_mutex_lock(philo->right_fork);
        print_action(philo, "has taken a fork", 0);
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
        print_action(philo, "has taken a fork", 0);
        pthread_mutex_lock(philo->left_fork);
        print_action(philo, "has taken a fork", 0);
    }
}
void drop_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}