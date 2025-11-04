#include "philo.h"

void    set_simulation_stopped(t_philo *philo)
{
    pthread_mutex_lock(&philo->rules->sim_end_mutex);
    philo->rules->simulation_stop = true;
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
    long    now;
    long    time_since_meal;

    // last_meal_time okuması kilit altında
    pthread_mutex_lock(&philo->meal_mutex);
    if (philo->is_eating) // Eğer şu anda yemek yiyorsa, ölmüş olamaz!
    {
        pthread_mutex_unlock(&philo->meal_mutex);
        return; 
    }
    now = get_time_in_ms();
    time_since_meal = now - philo->last_meal_time;
    pthread_mutex_unlock(&philo->meal_mutex);
    
    if (time_since_meal > philo->rules->time_to_die)
    {
        if (!is_simulation_stopped(philo->rules))
        {
            set_simulation_stopped(philo);
            
            // KRİTİK DÜZELTME: start_time okuması kilit altına alınıyor
            pthread_mutex_lock(&philo->rules->start_time_mutex); 
            long time = get_time_in_ms() - philo->rules->start_time;
            pthread_mutex_unlock(&philo->rules->start_time_mutex); 
            
            pthread_mutex_lock(&philo->rules->print_mutex);
            printf("%ld %d died\n", time, philo->id);
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
        // meals_eaten okuması kilit altında
        pthread_mutex_lock(&philo[i].meal_mutex);
        if (philo[i].meals_eaten >= philo->rules->num_must_eat)
            all_eaten_count++;
        pthread_mutex_unlock(&philo[i].meal_mutex);
        i++;
    }
    
    if (all_eaten_count == philo->rules->num_philos)
    {
        print_action(philo, "meals were eaten", 1);
        set_simulation_stopped(philo);
    }
}

void    *monitor_routine(void *arg)
{
    t_philo *philo;
    
    philo = (t_philo *)arg;
    
    // BAŞLATMA SENKRONİZASYONU: Main sinyali verene kadar bekle
    while (!philo->rules->start_flag)
        usleep(100); 

    while (!is_simulation_stopped(philo->rules))
    {
        int i = 0;
        
        check_all_eaten(philo);
        if(is_simulation_stopped(philo->rules))
            return NULL;
        while (i < philo->rules->num_philos)
        {
            check_philo_death(&philo[i]);
            if(is_simulation_stopped(philo->rules))
                return NULL;
            i++;
        }
        
        if(is_simulation_stopped(philo->rules))
            return NULL;
            
        // KRİTİK DÜZELTME: Monitör beklemesi (0.5ms)
        usleep(500); 
    }
    return (NULL);
}