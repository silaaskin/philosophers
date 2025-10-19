/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: silaaskin <silaaskin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:38:32 by silaaskin         #+#    #+#             */
/*   Updated: 2025/10/16 14:52:59 by silaaskin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void think(t_philo *philo)
{
    // Dinamik düşünme gecikmesi: çatal alma zamanlarını dağıtarak deadlock riskini azaltır
    print_action(philo, "is thinking", 0);
    usleep(1000 + (philo->id % philo->rules->num_philos) * 500);    
}



void    eat(t_philo *philo)
{
    pick_forks(philo);
    
    pthread_mutex_lock(philo->meal_mutex);
    philo->last_meal_time = get_time_in_ms();
    pthread_mutex_unlock(philo->meal_mutex);

    print_action(philo, "is eating", 0);

    usleep(philo->rules->time_to_eat * 1000);

    pthread_mutex_lock(philo->meal_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->meal_mutex);
    
    drop_forks(philo);
}
void    my_sleep(t_philo   *philo)
{
    print_action(philo, "is sleeping", 0);
    usleep(philo->rules->time_to_sleep * 1000);
}


void    *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_rules *rules = philo->rules;
    
    if (rules->num_philos == 1)
    {
        one_philo(philo);
        return NULL;
    }
    
    else if (philo->id % 2 == 0)
        usleep(1000 + (philo->id % rules->num_philos) * 500);

    while(!is_simulation_stopped(philo->rules))
    {
        think(philo);
        eat(philo);
        my_sleep(philo);
    }
    return NULL;
}
 