/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabid <moabid@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 21:38:26 by moabid            #+#    #+#             */
/*   Updated: 2022/07/05 16:33:29 by moabid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <inttypes.h>
# include <sys/time.h>

# define SYNTAX_ERROR		"[-] Syntax error :\
	./philo num_p time_d time_e time_s [philo_eat_num]\n"
# define DATA_ERROR			"Error : Initialization error.\n"
# define THREADING_ERROR	"Error : Thread error.\n"
# define INPUT_ERROR		"Error : Invalide Input.\n"



# define STR_THINKING	"is thinking"
# define STR_EATING		"is eating"
# define STR_SLEEPING	"is sleeping"
# define STR_TOOK_FORK	"has taken a fork"
# define STR_DEAD		"died"

enum {
	MAX = 2147483647,
	MIN = -2147483648
} limits;

typedef enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	TOOK_FORK,
	DEAD
} t_state;


struct philo {
	int 			id;
	int				l_fork;
	int				r_fork;
	int				eat_count;
	bool			done_eating;
	uint64_t		last_eat;
	pthread_t		thread_id;
	struct table	*table;
};

struct data {
	int 			num_p;
	int 			time_d;
	int 			time_e;
	int 			time_s;
	bool			is_running;
	uint64_t		start_time;
	int 			philo_eat_num;
};

struct table {
	struct philo		*philo;
	int					count_done;
	pthread_mutex_t		*forks;
	pthread_mutex_t		writer;
	struct data			data;
};


//////////////////////
//	   data		   //
/////////////////////




//////////////////////
//		utils		//
/////////////////////

size_t		ft_strlen(const char *s);
void		ft_error(char *error);
int			ft_atoi(const char *str);
void		*ft_malloc(size_t size);
void		print_state(struct philo *philo, t_state state);
void		m_sleep(uint64_t sleep_ms, struct philo *philo);
uint64_t	get_time_ms(struct philo *philo);
uint64_t	get_time_ms_table(struct table *table);

bool	threading(struct table *table);


bool	table_check_input(struct table *table);
bool	table_data_create(struct table *table, int argc, char *argv[]);
void	table_philos_init(struct table *table);
bool	table_create_philos(struct table *table);
bool	table_create_mutexes(struct table *table);
void 	table_create(struct table *table, int argc, char *argv[]);
void	table_destroy_threads(struct table *table);
void	table_destroy(struct table *table);


int		my_strcmp(const char *strg1, char *strg2);
void	philo_take_forks(struct philo *philo);
void	philo_leave_forks(struct philo *philo);
void	philo_eat(struct philo *philo);
void	philo_sleep(struct philo *philo);
void	philo_think(struct philo *philo);
void	*francisco_is_watching(void *arg);
void	*life_cycle(void *arg);



#endif