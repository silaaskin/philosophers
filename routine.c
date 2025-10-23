/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: silaaskin <silaaskin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:38:32 by silaaskin         #+#    #+#             */
/*   Updated: 2025/10/23 21:28:27 by silaaskin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "philo.h"

// void think(t_philo *philo)
// {
//     // Dinamik düşünme gecikmesi: çatal alma zamanlarını dağıtarak deadlock riskini azaltır
//     print_action(philo, "is thinking", 0);
//     usleep(1000);    

// }



// void    eat(t_philo *philo)
// {
//     long start_time_eat;

//     pick_forks(philo);
    
//     if (is_simulation_stopped(philo->rules)) 
//         return; 
    
//     print_action(philo, "is eating", 0);

//     pthread_mutex_lock(&philo->meal_mutex);
//     philo->last_meal_time = get_time_in_ms();
//     pthread_mutex_unlock(&philo->meal_mutex);
    
//     start_time_eat = get_time_in_ms();
//     while ((get_time_in_ms() - start_time_eat) < philo->rules->time_to_eat)
//     {
//         if (is_simulation_stopped(philo->rules))
//             break;
//         usleep(1000);
//     }

//     pthread_mutex_lock(&philo->meal_mutex);
//     philo->meals_eaten++;
//     pthread_mutex_unlock(&philo->meal_mutex);
    
//     drop_forks(philo);
// }
// void    my_sleep(t_philo   *philo)
// {
//     long start_time_sleep;

//     print_action(philo, "is sleeping", 0);
    
//     start_time_sleep = get_time_in_ms();
//     while ((get_time_in_ms() - start_time_sleep) < philo->rules->time_to_sleep)
//     {
//         if (is_simulation_stopped(philo->rules))
//             break;
//         usleep(1000);
//     }
// }


// void    *philo_routine(void *arg)
// {
//     t_philo *philo = (t_philo *)arg;
//     t_rules *rules;
//     rules = philo->rules;
    
//     if (rules->num_philos == 1)
//     {
//         one_philo(philo);
//         return NULL; 
//     }
//     if (philo->id % 2 != 0)
//         usleep(1000); 

//     while(!is_simulation_stopped(philo->rules))
//     {
//         pthread_mutex_lock(&philo->meal_mutex);
//         int meals_eaten_check = philo->meals_eaten;
//         pthread_mutex_unlock(&philo->meal_mutex);

//         if (rules->num_must_eat != -1 && meals_eaten_check >= rules->num_must_eat)
//             break;

//         think(philo);
//         eat(philo);
        
//         if (is_simulation_stopped(philo->rules)) 
//             break;
            
//         my_sleep(philo);
//     }
//     return NULL;
// }
 
/* ---------- routine.c ---------- */

#include "philo.h"

void	think(t_philo *philo)
{
	print_action(philo, "is thinking", 0);
	usleep(100);
}

// void	eat(t_philo *philo)
// {
// 	long	start_time_eat;

// 	pick_forks(philo);
// 	if (is_simulation_stopped(philo->rules))
// 		return;
// 	print_action(philo, "is eating", 0);
// 	pthread_mutex_lock(&philo->meal_mutex);
// 	philo->last_meal_time = get_time_in_ms();
// 	pthread_mutex_unlock(&philo->meal_mutex);
// 	start_time_eat = get_time_in_ms();
// 	while ((get_time_in_ms() - start_time_eat) < philo->rules->time_to_eat)
// 	{
// 		if (is_simulation_stopped(philo->rules))
// 			break;
// 		usleep(100);
// 	}
// 	pthread_mutex_lock(&philo->meal_mutex);
// 	philo->meals_eaten++;
// 	pthread_mutex_unlock(&philo->meal_mutex);
// 	drop_forks(philo);
// }

// void	my_sleep(t_philo *philo)
// {
// 	long	start_time_sleep;

// 	print_action(philo, "is sleeping", 0);
// 	start_time_sleep = get_time_in_ms();
// 	while ((get_time_in_ms() - start_time_sleep) < philo->rules->time_to_sleep)
// 	{
// 		if (is_simulation_stopped(philo->rules))
// 			break;
// 		usleep(100);
// 	}
// }
void eat(t_philo *philo)
{
    long start_time_eat;

    pick_forks(philo);
    if (is_simulation_stopped(philo->rules))
        return;

    print_action(philo, "is eating", 0);
    pthread_mutex_lock(&philo->meal_mutex);
    philo->last_meal_time = get_time_in_ms();
    pthread_mutex_unlock(&philo->meal_mutex);

    start_time_eat = get_time_in_ms();
    while ((get_time_in_ms() - start_time_eat) < philo->rules->time_to_eat)
    {
        if (is_simulation_stopped(philo->rules))
            break;
        if (philo->rules->time_to_eat > 5)
            usleep(200);  // CPU dostu kısa bekleme
        else
            usleep(50);   // çok kısa sürelerde hassas bekleme
    }

    pthread_mutex_lock(&philo->meal_mutex);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meal_mutex);

    drop_forks(philo);
}

void my_sleep(t_philo *philo)
{
    long start_time_sleep;

    print_action(philo, "is sleeping", 0);
    start_time_sleep = get_time_in_ms();
    while ((get_time_in_ms() - start_time_sleep) < philo->rules->time_to_sleep)
    {
        if (is_simulation_stopped(philo->rules))
            break;
        if (philo->rules->time_to_sleep > 5)
            usleep(200);  // CPU dostu bekleme
        else
            usleep(50);   // kısa süreler için hassas
    }
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;
	int		meals_eaten_check;

	philo = (t_philo *)arg;
	rules = philo->rules;
	if (rules->num_philos == 1)
	{
		one_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 != 0)
		usleep(100);
	while (!is_simulation_stopped(philo->rules))
	{
		pthread_mutex_lock(&philo->meal_mutex);
		meals_eaten_check = philo->meals_eaten;
		pthread_mutex_unlock(&philo->meal_mutex);
		if (rules->num_must_eat != -1 && meals_eaten_check >= rules->num_must_eat)
			break;
		think(philo);
		if (is_simulation_stopped(philo->rules))
			break;
		eat(philo);
		if (is_simulation_stopped(philo->rules))
			break;
		my_sleep(philo);
	}
	return (NULL);
}
