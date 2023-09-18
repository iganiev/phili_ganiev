/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_aeting.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alabdull <@student.42abudhabi.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 14:32:08 by iganiev           #+#    #+#             */
/*   Updated: 2023/09/18 22:33:08 by alabdull         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*buf;

	if (size && SIZE_MAX / size < count)
		return (NULL);
	buf = (void *)malloc(count * size);
	if (buf == NULL)
		return (buf);
	memset(buf, 0, count * size);
	return (buf);
}

void	display_routine(t_data *info, int id, char *message)
{
	long long	current_time;
	int			is_dead;

	current_time = get_time();
	pthread_mutex_lock(&(info->lock));
	is_dead = info->dead;
	if (!is_dead)
	{
		printf("%lld ", (current_time - info->timestamp));
		printf("%d ", id + 1);
		printf("%s\n", message);
	}
	pthread_mutex_unlock(&(info->lock));
}

void	grab_forks(t_philo *philo)
{
	// pthread_mutex_lock(&(philo->info->lock));
	// philo->info->fork_state[philo->left_fork] = 1;
	// philo->info->fork_state[philo->right_fork] = 1;
	// pthread_mutex_unlock(&(philo->info->lock));
	if (philo->info->count_philo % 2 != 0
		&& (philo->philo_id == philo->info->count_philo - 1))
	{
		pthread_mutex_lock(&(philo->info->forks[philo->right_fork]));
		pthread_mutex_lock(&(philo->info->forks[philo->left_fork]));
		pthread_mutex_lock(&(philo->info->lock));
		philo->info->fork_state[philo->right_fork] = 1;
		philo->info->fork_state[philo->left_fork] = 1;
		pthread_mutex_unlock(&(philo->info->lock));
	}
	else
	{
		pthread_mutex_lock(&(philo->info->forks[philo->left_fork]));
		pthread_mutex_lock(&(philo->info->forks[philo->right_fork]));
		pthread_mutex_lock(&(philo->info->lock));
		philo->info->fork_state[philo->left_fork] = 1;
		philo->info->fork_state[philo->right_fork] = 1;
		pthread_mutex_unlock(&(philo->info->lock));
	}
}

void	release_forks(t_philo *philo)
{
	// pthread_mutex_lock(&(philo->info->lock));
	// philo->info->fork_state[philo->left_fork] = 0;
	// philo->info->fork_state[philo->right_fork] = 0;
	// pthread_mutex_unlock(&(philo->info->lock));
	if (philo->info->count_philo % 2 != 0
		&& (philo->philo_id == philo->info->count_philo - 1))
	{
		pthread_mutex_unlock(&(philo->info->forks[philo->right_fork]));
		pthread_mutex_unlock(&(philo->info->forks[philo->left_fork]));
		pthread_mutex_lock(&(philo->info->lock));
		philo->info->fork_state[philo->right_fork] = 0;
		philo->info->fork_state[philo->left_fork] = 0;
		pthread_mutex_unlock(&(philo->info->lock));
	}
	else
	{
		pthread_mutex_unlock(&(philo->info->forks[philo->left_fork]));
		pthread_mutex_unlock(&(philo->info->forks[philo->right_fork]));
		pthread_mutex_lock(&(philo->info->lock));
		philo->info->fork_state[philo->left_fork] = 0;
		philo->info->fork_state[philo->right_fork] = 0;
		pthread_mutex_unlock(&(philo->info->lock));
	}
	// ft_usleep(philo->info->time_to_eat);
}

int	is_eating(t_philo *philo)
{
	if (philo->info->count_philo != 1)
	{
		grab_forks(philo);
		display_routine(philo->info, philo->philo_id, "has taken a fork");
		display_routine(philo->info, philo->philo_id, "has taken a fork");
		display_routine(philo->info, philo->philo_id, "is eating");
		pthread_mutex_lock(&(philo->ph_lock));
		philo->last_meal_time = get_time();
		philo->meal_count = philo->meal_count + 1;
		pthread_mutex_unlock(&(philo->ph_lock));
		ft_usleep(philo->info->time_to_eat);
		release_forks(philo);
	}
	return (0);
}
