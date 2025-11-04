#include "philo.h"

void    print_action(t_philo *philo, char *output, int flag)
{
    long    time;
    int     stopped;

    stopped = is_simulation_stopped(philo->rules);
    if (stopped)
        return;
        
    pthread_mutex_lock(&philo->rules->print_mutex);
    
    stopped = is_simulation_stopped(philo->rules);
    
    pthread_mutex_lock(&philo->rules->start_time_mutex);
    time = get_time_in_ms() - philo->rules->start_time;
    pthread_mutex_unlock(&philo->rules->start_time_mutex);
    
    if (!stopped && flag == 1)
    {
        int i = 0;
        printf("%d %s\n", philo[i].meals_eaten , output);
        i = i+1;
        printf("%d %s\n",philo[i].meals_eaten, output);
        i = i + 1;
        printf("%d %s\n", philo[i].meals_eaten, output);
        i = i + 1;
        printf("%d %s\n", philo[i].meals_eaten, output);
        i = i + 1;
        printf("%d %s\n", philo[i].meals_eaten, output);
        i = i + 1;
        printf("%d %s\n", philo[i].meals_eaten, output);

        set_simulation_stopped(philo);
    }
    else if (!stopped)
        printf("%ld %d %s\n", time, philo->id, output);
        
    pthread_mutex_unlock(&philo->rules->print_mutex);
}

void    *one_philo(t_philo *philo)
{
    pthread_mutex_lock(&philo->rules->start_lock);
    pthread_mutex_unlock(&philo->rules->start_lock);
    
    print_action(philo, "is thinking", 0);
    pthread_mutex_lock(philo->left_fork);
    print_action(philo, "has taken a fork", 0);
    
    ft_usleep(philo->rules->time_to_die, philo->rules); 
    
    pthread_mutex_unlock(philo->left_fork);
    return (NULL);
}

void pick_forks(t_philo *philo)
{
    if (is_simulation_stopped(philo->rules))
        return;
        
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->left_fork);
        print_action(philo, "has taken a fork", 0);
        
        if (is_simulation_stopped(philo->rules))
        { 
            pthread_mutex_unlock(philo->left_fork);
            return;
        }
        
        pthread_mutex_lock(philo->right_fork);
        print_action(philo, "has taken a fork", 0);
    }
    else
    {
        pthread_mutex_lock(philo->right_fork);
        print_action(philo, "has taken a fork", 0);
        
        if (is_simulation_stopped(philo->rules)) { pthread_mutex_unlock(philo->right_fork); return; }
        
        pthread_mutex_lock(philo->left_fork);
        print_action(philo, "has taken a fork", 0);
    }
    
    if (is_simulation_stopped(philo->rules))
        drop_forks(philo);
}

void drop_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}