/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabid <moabid@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 17:37:21 by moabid            #+#    #+#             */
/*   Updated: 2022/07/05 16:32:41 by moabid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_take_forks(struct philo *philo)
{
	pthread_mutex_lock(&philo->table->forks[philo->l_fork]);
	print_state(philo, TOOK_FORK);
	pthread_mutex_lock(&philo->table->forks[philo->r_fork]);
	print_state(philo, TOOK_FORK);
}

void	philo_leave_forks(struct philo *philo)
{
	pthread_mutex_unlock(&philo->table->forks[philo->l_fork]);
	pthread_mutex_unlock(&philo->table->forks[philo->r_fork]);
}

void	philo_eat(struct philo *philo)
{
	print_state(philo, EATING);
	philo->eat_count++;
	philo->last_eat = get_time_ms(philo);
	usleep(philo->table->data.time_e * 1000);
}

void	philo_sleep(struct philo *philo)
{
	print_state(philo, SLEEPING);
	usleep(philo->table->data.time_s * 1000);
}

void	philo_think(struct philo *philo)
{
		print_state(philo, THINKING);
}

uint64_t	get_time_ms_table(struct table *table)
{
	int				time_ms;
	struct timeval	thread_time;

	gettimeofday(&thread_time, NULL);
	time_ms = ((thread_time.tv_usec - table->data.start_time) / 1000)
		+ ((thread_time.tv_sec - table->data.start_time) * 1000);
	return (time_ms);
}

void	*francisco_is_watching(void *arg)
{
	int				index;
	uint64_t		timestamp;
	struct table	*table;

	table = (struct table *)arg;
	index = 0;
	while (true)
	{
		if (table->count_done == table->data.num_p)
			break ;
		if (index == table->data.num_p)
			index = 0;
		usleep(1000);
		timestamp = get_time_ms_table(table);
		if (!table->philo[index].done_eating
			&& (int)(timestamp - table->philo[index].last_eat) > table->data.time_d)
		{
			
			print_state(&table->philo[index], DEAD);
			table->data.is_running = false;
			break ;
		}
		index++;
	}
	return (NULL);
}

void	*life_cycle(void *arg)
{
	struct philo	*philo;

	// while("all are created")
	philo = (struct philo *)arg;
	philo->last_eat = get_time_ms(philo);
	if (philo->id % 2 == 0)
	{
		usleep(100);
	}
	while (philo->table->data.is_running)
	{
		philo_take_forks(philo);
		philo_eat(philo);
		philo_leave_forks(philo);
		philo_think(philo);
		if (philo->eat_count == philo->table->data.philo_eat_num)
		{
			philo->done_eating = true;
			philo->table->count_done++;
			break ;
		}
		philo_sleep(philo);
	}
	return (NULL);
}