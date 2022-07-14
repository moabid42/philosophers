/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabid <moabid@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 21:41:56 by moabid            #+#    #+#             */
/*   Updated: 2022/07/05 16:31:43 by moabid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr)
		memset(ptr, '0', size);
	else
	{
		free(ptr);
		return (NULL);
	}
	return (ptr);
}

void	ft_error(char *error)
{
	write(2, error, ft_strlen(error));
	exit(1);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i++] != '\0')
		;
	return (i - 1);
}

static int	ft_isspace(int c)
{
	return (c == '\f' || c == '\n' || c == '\r' \
	|| c == '\t' || c == '\v' || c == ' ');
}

static int ft_isend(int c)
{
	return (c == 0);
}

int	ft_atoi(const char *str)
{
	long long	num;

	num = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
		ft_error(INPUT_ERROR);
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
		num = num * 10 + *str++ - '0';
	if ((num > MAX) || !ft_isend(*str))
		ft_error(INPUT_ERROR);
	return (num);
}


// uint64_t	get_time_ms(void)
// {
// 	static struct timeval	tv;

// 	gettimeofday(&tv, NULL);
// 	return (init_time.tv_sec - curr.tv_sec) * 1000 + (init_time.tv_usec - curr.tv_usec / 1000)
// 	// return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
// }

uint64_t	get_time_ms(struct philo *philo)
{
	int				time_ms;
	struct timeval	thread_time;

	gettimeofday(&thread_time, NULL);
	time_ms = ((thread_time.tv_usec - philo->table->data.start_time) / 1000)
		+ ((thread_time.tv_sec - philo->table->data.start_time) * 1000);
	return (time_ms);
}



void	m_sleep(uint64_t sleep_ms, struct philo *philo)
{
	uint64_t	end_time;

	end_time = get_time_ms(philo) + sleep_ms;
	while (get_time_ms(philo) < end_time && philo->table->data.is_running > 0)
		usleep(50);
}

void	print_state(struct philo *philo, t_state state)
{
	const char	*states[6] = {STR_THINKING, STR_EATING, STR_SLEEPING,
		STR_TOOK_FORK, STR_DEAD, NULL};
	uint64_t timestamp;

	timestamp = get_time_ms(philo);
	if (!philo->table->data.is_running)
		return ;
	pthread_mutex_lock(&philo->table->writer);
	if (!my_strcmp(states[state], STR_EATING))
		printf("\x1B[31m%-5llu \x1B[36m[%d] \x1B[32m%-20s\x1B[37m​​ 🍕\n", timestamp, philo->id, states[state]);
	else if (!my_strcmp(states[state], STR_THINKING))
		printf("\x1B[31m%-5llu \x1B[36m[%d] \x1B[32m%-20s\x1B[37m ​💬​​\n", timestamp, philo->id, states[state]);
	else if (!my_strcmp(states[state], STR_SLEEPING))
		printf("\x1B[31m%-5llu \x1B[36m[%d] \x1B[32m%-20s\x1B[37m​ 💤​​​\n", timestamp, philo->id, states[state]);
	else if (!my_strcmp(states[state], STR_TOOK_FORK))
		printf("\x1B[31m%-5llu \x1B[36m[%d] \x1B[32m%-20s\x1B[37m 🥄​\n", timestamp, philo->id, states[state]);
	else if (!my_strcmp(states[state], STR_DEAD))
		printf("\x1B[31m%-5llu \x1B[36m[%d] \x1B[32m%-20s\x1B[37m​ 💀​​​\n", timestamp, philo->id, states[state]);
	pthread_mutex_unlock(&philo->table->writer);
}

int	my_strcmp(const char *strg1, char *strg2)
{
	while ((*strg1 != '\0' && *strg2 != '\0') && *strg1 == *strg2)
	{
		strg1++;
		strg2++;
	}
	if (*strg1 == *strg2)
		return (0);
	else
		return (*strg1 - *strg2);
}
