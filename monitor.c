/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: silaaskin <silaaskin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:54:59 by silaaskin         #+#    #+#             */
/*   Updated: 2025/10/16 23:14:11 by silaaskin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    set_simulation_stopped(t_philo *philo)
{
    pthread_mutex_lock(&philo->rules->sim_end_mutex);
    philo->rules->simulation_stop = 1;
    pthread_mutex_unlock(&philo->rules->sim_end_mutex);
}
int     is_simulation_stopped(t_rules *rules)
{
    int end;
    pthread_mutex_lock(&rules->sim_end_mutex);
    end = rules->simulation_stop;
    pthread_mutex_unlock(&rules->sim_end_mutex);
    return end;
}

void    check_philo_death(t_philo *philo)
{
    long now;
    long time_since_meal;

    pthread_mutex_lock(philo->meal_mutex);
    now = get_time_in_ms();
    time_since_meal = now - philo->last_meal_time;
    pthread_mutex_unlock(philo->meal_mutex);
    
    if (time_since_meal >= philo->rules->time_to_die)// = i çıkarman gerekebilir
    {
        print_action(philo, "died", 0);
        set_simulation_stopped(philo);
    }
}
void    check_all_eaten(t_philo *philo)
{
    int all_eaten_count;
    int i;
    
    all_eaten_count = 0;
    i = 0;
    while (i < philo->rules->num_philos)
    {
        pthread_mutex_lock(philo[i].meal_mutex);
        if (philo[i].meals_eaten >= philo->rules->num_must_eat)//  && philo->rules->num_must_eat != -1 buna gerek var mı bilmiyorum 
            all_eaten_count++;
        pthread_mutex_unlock(philo[i].meal_mutex);
        i++;
    }
    if (all_eaten_count == philo->rules->num_must_eat)
    {
        print_action(philo, "meals were eating", 1);
        set_simulation_stopped(philo);
    }
}
void    *monitor_routine(void *arg)
{
    t_philo *philo;
    int     i;
    
    philo = (t_philo *)arg;
    while (!is_simulation_stopped(philo->rules))
    {
        check_all_eaten(philo);
        i = 0;
        while (i < philo->rules->num_philos)
        {
            check_philo_death(&philo[i]);
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}
 