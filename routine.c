#include "philo.h"

void    think(t_philo *philo)
{
    print_action(philo, "is thinking", 0);
}

int eat(t_philo *philo)
{
    pick_forks(philo);
    
    if (is_simulation_stopped(philo->rules))
        return 1;
        
    pthread_mutex_lock(&philo->meal_mutex);
    philo->is_eating = true;
    philo->last_meal_time = get_time_in_ms();
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->meal_mutex);
        
    print_action(philo, "is eating", 0);
    
    // last_meal ve meals_eaten kilit altında güncelleniyor

    ft_usleep(philo->rules->time_to_eat, philo->rules);
    pthread_mutex_lock(&philo->meal_mutex);
    philo->is_eating = false; // Yemeyi BİTİRDİ
    pthread_mutex_unlock(&philo->meal_mutex);
    
    if (is_simulation_stopped(philo->rules))
    {
        drop_forks(philo); 
        return 1;   
    }

    
    drop_forks(philo);
    return 0;
}

void my_sleep(t_philo *philo)
{
    int is_sim_stop;
    is_sim_stop = is_simulation_stopped(philo->rules);
    if(!is_sim_stop)
    {
        print_action(philo, "is sleeping", 0);
        ft_usleep(philo->rules->time_to_sleep, philo->rules);
    }
}

void    *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_rules *rules = philo->rules;
    
    // BAŞLATMA SENKRONİZASYONU: Main'deki kilit açılana kadar bekle
    pthread_mutex_lock(&rules->start_lock);
    pthread_mutex_unlock(&rules->start_lock);
    
    if (rules->num_philos == 1)
        return one_philo(philo);
        
    if (philo->id % 2 != 0)
        usleep(100); 

    while (!is_simulation_stopped(philo->rules))
    {
        // Yemek sayısını kontrol et
        pthread_mutex_lock(&philo->meal_mutex);
        int meals_eaten_check = philo->meals_eaten;
        pthread_mutex_unlock(&philo->meal_mutex);

        if (rules->num_must_eat != -1 && meals_eaten_check >= rules->num_must_eat)
            break;

        // Rutin
        if (is_simulation_stopped(philo->rules)) 
            break;
        think(philo);
        if (is_simulation_stopped(philo->rules))
            break; 
        if(eat(philo))
            return NULL;
        if (is_simulation_stopped(philo->rules))
            break;
        my_sleep(philo);
    }
    return NULL;
}