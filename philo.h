#ifndef PHILO_H_
# define PHILO_H_

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

// Yapı Tanımları
typedef struct  s_rules
{
    int                 num_philos;
    int                 time_to_die;
    int                 time_to_eat;
    int                 time_to_sleep;
    int                 num_must_eat;
    pthread_mutex_t     *forks;
    pthread_mutex_t     print_mutex;
    
    // START_TIME ve ONU KORUYAN MUTEX (Data Race Çözümü)
    long                start_time;
    pthread_mutex_t     start_time_mutex; 
    
    bool                simulation_stop;
    pthread_mutex_t     sim_end_mutex;
    
    // BAŞLATMA SENKRONİZASYONU
    pthread_mutex_t     start_lock;
    bool                start_flag;

}   t_rules;

typedef struct  s_philo
{
    int             id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    int             meals_eaten;
    long            last_meal_time;
    t_rules         *rules;
    pthread_t       thread;
    pthread_mutex_t meal_mutex; // last_meal_time ve meals_eaten için
    bool    is_eating; // YENİ BAYRAK
}   t_philo;

// Fonksiyon Prototipleri
long    get_time_in_ms(void);
t_philo *init_philos(t_rules *rules);
void    create_threads(t_philo *philo, t_rules *rules, pthread_t *monitor_thread);
void    init_forks(t_rules *rules);
int     check_arg(int argc, char **argv);
void    init_structers(int argc, char **argv, t_rules *rules);
void    print_action(t_philo *philo, char *output, int flag);
void    *one_philo(t_philo *philo);
void    pick_forks(t_philo  *philo);
void    drop_forks(t_philo *philo);
int     eat(t_philo *philo);
void    my_sleep(t_philo   *philo);
void    set_simulation_stopped(t_philo *philo);
void    check_philo_death(t_philo *philo);
void    check_all_eaten(t_philo *philo);
void    *philo_routine(void  *arg);
int     is_simulation_stopped(t_rules *rules);
void    *monitor_routine(void *arg);
long    ft_atoi(const char *str);
void    ft_usleep(long time_ms, t_rules *rules); 
int     error_message(char  *error_message);

#endif