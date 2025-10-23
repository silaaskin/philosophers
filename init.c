/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: silaaskin <silaaskin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 18:34:09 by saskin            #+#    #+#             */
/*   Updated: 2025/10/23 20:04:59 by silaaskin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    init_philos(t_philo *philos, t_rules *rules)
{
    int i;
    i = -1;
    
	while(++i < rules->num_philos)
    {
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
        philos[i].id = i + 1;
        philos[i].meals_eaten = 0;
        philos[i].rules = rules;
        philos[i].left_fork = &rules->forks[i];
        philos[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];//dairesel şekilde olabilmesi için
    }

}

void    init_forks(t_rules *rules)
{
    
    int	i;

    rules->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * rules->num_philos);
	if(!rules->forks)
		exit (1);	
    i = 0;
    while(i < rules->num_philos)
    {
        pthread_mutex_init(&rules->forks[i], NULL);// aynı bu döngü gibi döngü ile destroy edip en son rules->forks freelenmeli
        i++;
    }
}
void    init_structers(int argc, char **argv, t_rules *rules)
{
    rules->num_philos = ft_atoi(argv[1]);
    rules->time_to_die = ft_atoi(argv[2]);
    rules->time_to_eat = ft_atoi(argv[3]);
    rules->time_to_sleep = ft_atoi(argv[4]);
    pthread_mutex_init(&rules->sim_end_mutex, NULL);//program sonunda destroy yapman lazım
    pthread_mutex_init(&rules->print_mutex, NULL);
    rules->simulation_stop = 0;
    if (argc == 6)
        rules->num_must_eat = ft_atoi(argv[5]);
    else if (argc == 5)
        rules->num_must_eat = -1;
}
void    init_thread(t_philo *philo)
{
    int i;
    i = -1;
    while (++i < philo->rules->num_philos)
        pthread_create(&philo[i].thread, NULL, &philo_routine, &philo[i]);//hepsi için ayrı threadler oluşturdum ve philo_routine i çalıştırdım
} 