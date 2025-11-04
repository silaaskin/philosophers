#include "philo.h"

t_philo *init_philos(t_rules *rules)
{
    int i;
    t_philo *philo;
    
    philo = (t_philo *)malloc(sizeof(t_philo) * rules->num_philos);
    if (!philo)
    {
        error_message("Malloc error");
        return NULL;
    }
    i = -1;
    while(++i < rules->num_philos)
    {
        pthread_mutex_init(&philo[i].meal_mutex, NULL);
        philo[i].id = i + 1;
        philo[i].meals_eaten = 0;
        philo[i].left_fork = &rules->forks[i];
        philo[i].right_fork = &rules->forks[(i + 1) % rules->num_philos];
        philo[i].rules = rules;
        philo[i].last_meal_time = 0; 
        philo[i].is_eating = 0;
    }
    return (philo);
}

void    init_forks(t_rules *rules)
{
    int i;
    rules->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * rules->num_philos);
    if(!rules->forks)
    {
        error_message("Fork malloc error\n");
        exit (1);   
    }
    i = 0;
    while(i < rules->num_philos)
    {
        pthread_mutex_init(&rules->forks[i], NULL);
        i++;
    }
}

void    init_structers(int argc, char **argv, t_rules *rules)
{
    rules->num_philos = ft_atoi(argv[1]);
    rules->time_to_die = ft_atoi(argv[2]);
    rules->time_to_eat = ft_atoi(argv[3]);
    rules->time_to_sleep = ft_atoi(argv[4]);
    
    pthread_mutex_init(&rules->sim_end_mutex, NULL);
    pthread_mutex_init(&rules->print_mutex, NULL);
    
    // BAŞLATMA VE DATA RACE ÇÖZÜMÜ MUTEX'LERİ
    pthread_mutex_init(&rules->start_lock, NULL);
    pthread_mutex_init(&rules->start_time_mutex, NULL); // YENİ MUTEX
    rules->start_flag = false;
    
    // Başlatma kilidi alınıyor
    pthread_mutex_lock(&rules->start_lock); 
    
    rules->simulation_stop = 0;
    if (argc == 6)
        rules->num_must_eat = ft_atoi(argv[5]);
    else if (argc == 5)
        rules->num_must_eat = -1;
}