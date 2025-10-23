/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: silaaskin <silaaskin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 17:54:59 by silaaskin         #+#    #+#             */
/*   Updated: 2025/10/23 21:31:38 by silaaskin        ###   ########.fr       */
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

void	check_philo_death(t_philo *philo)
{
	long	now;
	long	expected_death;
	long	time_to_print;

	pthread_mutex_lock(&philo->meal_mutex);
	expected_death = philo->last_meal_time + philo->rules->time_to_die;
	pthread_mutex_unlock(&philo->meal_mutex);

	now = get_time_in_ms();

	if (now >= expected_death)
	{
		if (!is_simulation_stopped(philo->rules))
		{
			set_simulation_stopped(philo);

			time_to_print = expected_death - philo->rules->start_time;
			pthread_mutex_lock(&philo->rules->print_mutex);
			printf("%ld %d died\n", time_to_print, philo->id);
			pthread_mutex_unlock(&philo->rules->print_mutex);
		}
	}
}

void check_all_eaten(t_philo *philo)
{
    int all_eaten_count;
    int i;
    
    if (philo->rules->num_must_eat == -1)
        return;

    all_eaten_count = 0;
    i = 0;
    
    while (i < philo->rules->num_philos)
    {
        pthread_mutex_lock(&philo[i].meal_mutex);
        
        if (philo[i].meals_eaten >= philo->rules->num_must_eat)
            all_eaten_count++;
            
        pthread_mutex_unlock(&philo[i].meal_mutex);
        i++;
    }
    
    if (all_eaten_count == philo->rules->num_philos)
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
        i = 0;
        
        while (i < philo->rules->num_philos)
        {
            check_philo_death(&philo[i]);
            
            if(is_simulation_stopped(philo->rules))
                return NULL;
            i++;
        }
        
        check_all_eaten(philo);
        if(is_simulation_stopped(philo->rules))
            return NULL;
        usleep(100); 
    }
    return (NULL);
}
 