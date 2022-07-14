/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabid <moabid@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 21:38:13 by moabid            #+#    #+#             */
/*   Updated: 2022/07/04 23:23:53 by moabid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool threading(struct table *table)
{
	int			index;
	pthread_t	francisco_thread_id;

	index = 0;
	while (index < table->data.num_p)
	{
		if (pthread_create(&table->philo[index].thread_id, NULL,
			&life_cycle, &table->philo[index]) != 0)
			return (false);
		index++;
	}
	pthread_create(&francisco_thread_id, NULL, &francisco_is_watching, table);
	pthread_join(francisco_thread_id, NULL);
	index = 0;
	while (index < table->data.num_p)
	{
		if (pthread_join(table->philo[index].thread_id, NULL) != 0)
			return (false);
		index++;
	}
	return (true);
}

int main(int argc, char *argv[])
{
	struct table	table;

	if (argc != 5 && argc != 6)
		ft_error(SYNTAX_ERROR);

	table_create(&table, argc, argv);
	if (!threading(&table))
		ft_error(THREADING_ERROR);
	table_destroy(&table);

	return (0);
}

