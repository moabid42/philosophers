/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabid <moabid@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 02:17:09 by moabid            #+#    #+#             */
/*   Updated: 2022/07/05 16:33:17 by moabid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	table_check_input(struct table *table)
{
	return (table->data.num_p < 1 
		|| table->data.time_d < 1 
		|| table->data.time_e < 1 
		|| table->data.time_s < 1);
}

bool	table_data_create(struct table *table, int argc, char *argv[])
{
	table->data.is_running = true;
	table->data.start_time = get_time_ms_table(table);;
	printf("-------------->%llu\n", table->data.start_time);
	table->data.num_p  = ft_atoi(argv[1]);
	table->data.time_d = ft_atoi(argv[2]);
	table->data.time_e = ft_atoi(argv[3]);
	table->data.time_s = ft_atoi(argv[4]);
	if (argc == 6)
		table->data.philo_eat_num = ft_atoi(argv[5]);
	else
		table->data.philo_eat_num = -1;
	if (table_check_input(table))
		return (false);
	return (true);
}

void	table_philos_init(struct table *table)
{
	int	index;

	index = 0;
	while (index < table->data.num_p)
	{
		table->philo[index].id = index + 1;
		table->philo[index].l_fork = index;
		if (index + 1 == table->data.num_p)
			table->philo[index].r_fork = 0;
		else
			table->philo[index].r_fork = (index + 1);
		table->philo[index].thread_id = 0;
		table->philo[index].last_eat = 0;
		table->philo[index].eat_count = 0;
		table->philo[index].done_eating = false;
		table->philo[index].table = table;
		index++;
	}
}

bool	table_create_philos(struct table *table)
{
	table->philo = ft_malloc(sizeof(struct philo) * table->data.num_p);
	if (!table->philo)
		return (false);
	table_philos_init(table);
	return (true);
}

bool	table_create_mutexes(struct table *table)
{
	int	index;

	index = 0;
	table->forks = ft_malloc(sizeof(pthread_mutex_t) * table->data.num_p);
	if (!table->forks)
		return (false);
	while (index < table->data.num_p)
	{
		if (pthread_mutex_init(&table->forks[index], NULL) != 0)
			return (false);
		index++;
	}
	if (pthread_mutex_init(&table->writer, NULL) != 0)
		return (false);
	return (true);
}

void 	table_create(struct table *table, int argc, char *argv[])
{
	if (table_data_create(table, argc, argv))
		if (table_create_philos(table))
			if (table_create_mutexes(table))
				return ;
	ft_error(DATA_ERROR);
}

void	table_destroy_threads(struct table *table)
{
	int	index;

	index = 0;
	while (index < table->data.num_p)
	{
		pthread_detach(table->philo[index].thread_id);
		index++;
	}
}

void	table_destroy(struct table *table)
{
	int	index;

	index = 0;
	table_destroy_threads(table);
	while (index < table->data.num_p)
	{
		pthread_mutex_destroy(&table->forks[index]);
		index++;
	}
	pthread_mutex_destroy(&table->writer);
	free(table->philo);
	free(table->forks);
}