#include "philo.h"

// GÜNCELLENDİ: Bekleme sırasında simülasyon kontrolü yapar
void    ft_usleep(long time_ms, t_rules *rules) 
{
    long start_time;
    long end_time;
    
    start_time = get_time_in_ms();
    end_time = start_time + time_ms;
    
    while (get_time_in_ms() < end_time)
    {
        if (is_simulation_stopped(rules))
            return;
        usleep(500); 
    }
}

long    get_time_in_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

// Orijinal check_arg fonksiyonunuz
int    check_arg(int argc, char **argv)
{
    if((argc != 5 && argc != 6) || ft_atoi(argv[1]) > 200 ||  ft_atoi(argv[2]) <= 0 ||
    ft_atoi(argv[3]) <= 0 || ft_atoi(argv[4]) <= 0)
        return (1);
    return (0);
}

// ... (ft_atoi ve error_message eklenmeli)

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
    
    // BAŞLATMA VE START_TIME MUTEX'LERİ TEMİZLENİYOR
    pthread_mutex_destroy(&philo->rules->start_lock);
    pthread_mutex_destroy(&philo->rules->start_time_mutex); 
    
    i = -1;
    while (++i < philo->rules->num_philos)
        pthread_mutex_destroy(&philo->rules->forks[i]);
    
    pthread_mutex_destroy(&philo->rules->print_mutex);
    pthread_mutex_destroy(&philo->rules->sim_end_mutex);
    
    i = -1;
    while (++i < philo->rules->num_philos)
        pthread_mutex_destroy(&philo[i].meal_mutex);
        
    free(philo->rules->forks);
    free(philo);
}

void create_threads(t_philo *philo, t_rules *rules, pthread_t *monitor_thread)
{
    int i = 0;

    while (i < rules->num_philos)
    {
        pthread_create(&philo[i].thread, NULL, &philo_routine, &philo[i]);
        usleep(100); 
        i++;
    }
    pthread_create(monitor_thread, NULL, &monitor_routine, philo);
}

int main(int argc, char **argv)
{
    t_rules rules;
    t_philo *philo;
    pthread_t monitor_thread;

    if (check_arg(argc, argv) != 0)
        return (error_message("Invalid number of arguments."));

    init_structers(argc, argv, &rules);
    init_forks(&rules);
    philo = init_philos(&rules);
    
    create_threads(philo, &rules, &monitor_thread);
    
    // === KRİTİK BAŞLATMA VE SENKRONİZASYON BÖLÜMÜ ===
    
    // 1. start_time'ı korumalı olarak kaydet
    pthread_mutex_lock(&rules.start_time_mutex);
    rules.start_time = get_time_in_ms();
    pthread_mutex_unlock(&rules.start_time_mutex);
    
    // 2. last_meal_time'ı başlatma zamanına eşitle
    int i = -1;
    while (++i < rules.num_philos)
    {
        pthread_mutex_lock(&philo[i].meal_mutex);
        philo[i].last_meal_time = rules.start_time;
        pthread_mutex_unlock(&philo[i].meal_mutex);
    }
        
    // 3. Kilit bayrağını ve kilidi serbest bırakarak tüm filozoflara BAŞLA sinyalini ver
    rules.start_flag = true;
    pthread_mutex_unlock(&rules.start_lock); 
    
    wait_join(monitor_thread, philo);
    wait_destroy(philo);
    return (0);
}