 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saskin <saskin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 16:27:47 by silaaskin         #+#    #+#             */
/*   Updated: 2025/10/11 18:36:15 by saskin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long    get_time_in_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
int    check_arg(int argc, char **argv)
{
    if((argc != 5 && argc != 6) || ft_atoi(argv[1]) > 200 ||  ft_atoi(argv[2]) <= 0 ||
    ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
        return (1);
    return (0);
}
void    wait_join(pthread_t monitor_thread, t_philo *philo)
{
    int i;
    i = -1;
    while (++i < philo->rules->num_philos)
        pthread_join(philo[i].thread, NULL);
    pthread_join(monitor_thread, NULL);
}

void    wait_destroy(t_philo *philo)
{
    int i;
    i = -1;
    while (++i < philo->rules->num_philos)
    
        pthread_mutex_destroy(&philo->rules->forks[i]);
    pthread_mutex_destroy(&philo->rules->print_mutex);
    pthread_mutex_destroy(&philo->rules->sim_end_mutex);
    i = -1;
    while (++i < philo->rules->num_philos)
    pthread_mutex_destroy(philo[i].meal_mutex);
    free(philo->rules->forks);
    free(philo);
    free(philo->rules);  
}

int main(int argc, char **argv)
{
    t_rules *rules;
    t_philo *philos;
    pthread_t monitor_thread;
    int i;

    if (check_arg(argc, argv) != 0)
        return (1);

    rules = (t_rules *)malloc(sizeof(t_rules));//free
    if (!rules)
        return (1);
    init_structers(argc, argv, rules);
     philos = (t_philo *)malloc(sizeof(t_philo) * rules->num_philos);//free
     if (!philos)
        return (1);
    init_forks(rules);//hata durumunda exit ms mi yapmalıyım int döndürüp burada mı işlemeliyim
    init_philos(philos, rules);
    
    i = -1;
    while (++i < rules->num_philos)
        pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]); 
    pthread_create(&monitor_thread, NULL, &monitor_routine, philos);
wait_join(monitor_thread, philos);
wait_destroy(philos);
    return (0);
}
