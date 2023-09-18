/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganiev <iganiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 23:07:56 by iganiev           #+#    #+#             */
/*   Updated: 2023/08/30 18:14:45 by iganiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(int ac, char *av[])
{
	int	i;

	if (ac != 5 && ac != 6)
	{
		printf("Error: Invalid number of arguments.\n");
		return (-1);
	}
	i = 1;
	while (av[i])
	{
		if (philo_atoi(av[i]) <= 0)
		{
			printf("Error: Invalid arguments.\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

void	get_data(t_data *info, int ac, char *av[])
{
	info->timestamp = get_time();
	info->count_philo = philo_atoi(av[1]);
	info->time_to_die = philo_atoi(av[2]);
	info->time_to_eat = philo_atoi(av[3]);
	info->time_to_sleep = philo_atoi(av[4]);
	if (ac == 6)
		info->count_meals = philo_atoi(av[5]);
	else
		info->count_meals = -1;
}

void	initialize_forks(t_data *info)
{
	int	i;

	if (pthread_mutex_init(&(info->lock), NULL))
	{
		free(info);
		printf("Error1\n");
		exit(1);
	}
	info->forks = malloc(sizeof(pthread_mutex_t) * info->count_philo);
	if (!(info->forks))
	{
		free(info);
		printf("Error2\n");
		exit(1);
	}
	i = 0;
	while (i < info->count_philo)
	{
		if (pthread_mutex_init(&(info->forks[i]), NULL))
		{
			free(info);
			printf("Error3\n");
			exit(1);
		}
		i++;
	}
}

void	initialize_philosophers(t_philo **philo, t_data *info)
{
	int	i;

	*philo = malloc(sizeof(t_philo) * info->count_philo);
	if (!(philo))
	{
		free(info);
		printf("Error4\n");
		exit(1);
	}
	info->fork_owners = malloc(sizeof(int) * info->count_philo);
    if (!(info->fork_owners))
    {
        free(*philo);
        free(info);
        printf("Error5\n");
        exit(1);
    }
	i = 0;
	while (i < info->count_philo)
	{
		(*philo)[i].philo_id = i;
		(*philo)[i].info = info;
		(*philo)[i].left_fork = i;
		(*philo)[i].meal_count = 0;
		(*philo)[i].last_meal_time = get_time();
		if (pthread_mutex_init(&((*philo)[i].ph_lock), NULL))
		{
			free(info);
			printf("Error6\n");
			exit(1);
		}
		info->fork_owners[i] = 0;
		(*philo)[i].right_fork = ((i + 1) % info->count_philo);
		if (info->count_philo % 2 == 0 && i == info->count_philo - 1)
		{
			(*philo)[i].right_fork = i;
			(*philo)[i].left_fork = ((i + 1) % info->count_philo);
		}
		i++;
	}
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

// boolean type holding fork set to yes
/* && philo left fork != philo_id && philo rigt fork != philo id*/
int	is_eating(t_philo *philo)
{
	// pthread_mutex_lock(&(philo->info->forks[philo->left_fork]));
	// pthread_mutex_lock(&(philo->ph_lock));
	// philo->holding_fork = true;
	// pthread_mutex_unlock(&(philo->ph_lock));
	// display_routine(philo->info, philo->philo_id, "has taken a fork");
	pthread_mutex_lock(&(philo->ph_lock));
	// pthread_mutex_lock(&(philo->info->lock));
	if (philo->info->count_philo != 1 && (philo->info->fork_owners[philo->left_fork] == 0) && (philo->info->fork_owners[philo->right_fork] == 0))
	{
		pthread_mutex_unlock(&(philo->ph_lock));
		// pthread_mutex_unlock(&(philo->info->lock));
		pthread_mutex_lock(&(philo->info->forks[philo->left_fork]));
		pthread_mutex_lock(&(philo->info->forks[philo->right_fork]));

		pthread_mutex_lock(&(philo->ph_lock));
		philo->info->fork_owners[philo->left_fork] = philo->philo_id;
		philo->info->fork_owners[philo->right_fork] = philo->philo_id;
		pthread_mutex_unlock(&(philo->ph_lock));

		display_routine(philo->info, philo->philo_id, "has taken a fork");
		display_routine(philo->info, philo->philo_id, "has taken a fork");

		display_routine(philo->info, philo->philo_id, "is eating");

		pthread_mutex_lock(&(philo->ph_lock));
		philo->last_meal_time = get_time();
		philo->meal_count = philo->meal_count + 1;
		philo->info->fork_owners[philo->left_fork] = 0;
		philo->info->fork_owners[philo->right_fork] = 0;
		pthread_mutex_unlock(&(philo->ph_lock));

		ft_usleep(philo->info->time_to_eat);
		pthread_mutex_unlock(&(philo->info->forks[philo->right_fork]));
		pthread_mutex_unlock(&(philo->info->forks[philo->left_fork]));
	}
	else
        pthread_mutex_unlock(&(philo->ph_lock));
	// philo->holding_fork = false;
	// pthread_mutex_unlock(&(philo->ph_lock));
	return (0);
}

// int is_eating(t_philo *philo)
// {
//     pthread_mutex_lock(&(philo->ph_lock)); // Lock the philosopher's lock

//     if (philo->info->count_philo != 1 &&
//         philo->info->fork_owners[philo->left_fork] == 0 &&
//         philo->info->fork_owners[philo->right_fork] == 0)
//     {
//         pthread_mutex_unlock(&(philo->ph_lock)); // Unlock the philosopher's lock

//         pthread_mutex_lock(&(philo->info->forks[philo->left_fork]));
//         pthread_mutex_lock(&(philo->info->forks[philo->right_fork]));

//         pthread_mutex_lock(&(philo->ph_lock)); // Lock the philosopher's lock
//         philo->info->fork_owners[philo->left_fork] = philo->philo_id;
//         philo->info->fork_owners[philo->right_fork] = philo->philo_id;
//         pthread_mutex_unlock(&(philo->ph_lock)); // Unlock the philosopher's lock

//         display_routine(philo->info, philo->philo_id, "has taken a fork");
//         display_routine(philo->info, philo->philo_id, "has taken a fork");
//         display_routine(philo->info, philo->philo_id, "is eating");

//         philo->last_meal_time = get_time();

//         pthread_mutex_lock(&(philo->ph_lock)); // Lock the philosopher's lock
//         philo->meal_count = philo->meal_count + 1;
//         philo->info->fork_owners[philo->left_fork] = 0;
//         philo->info->fork_owners[philo->right_fork] = 0;
//         pthread_mutex_unlock(&(philo->ph_lock)); // Unlock the philosopher's lock

//         ft_usleep(philo->info->time_to_eat);

//         pthread_mutex_unlock(&(philo->info->forks[philo->right_fork]));
//         pthread_mutex_unlock(&(philo->info->forks[philo->left_fork]));
//     }
//     else
//     {
//         pthread_mutex_unlock(&(philo->ph_lock)); // Unlock the philosopher's lock
//     }

//     return (0);
// }


int	is_sleeping(t_philo *philo)
{
	display_routine(philo->info, philo->philo_id, "is sleeping");
	ft_usleep(philo->info->time_to_sleep);
	return (0);
}

int	is_thinking(t_philo *philo)
{
	display_routine(philo->info, philo->philo_id, "is thinking");
	return (0);
}

int	finished_meals(t_data *info, t_philo *philo)
{
	int i = 0;
	int all_ate = 1;
	while (i < info->count_philo)
	{
		pthread_mutex_lock(&(philo[i].ph_lock));
		if (info->count_meals != -1 && philo[i].meal_count < info->count_meals)
		{
			all_ate = 0;
			pthread_mutex_unlock(&(philo[i].ph_lock));
			return (1);
		}
		pthread_mutex_unlock(&(philo[i].ph_lock));
		i++;
	}
	if (all_ate == 1 && info->count_meals != -1)
		return 1;
		// i = 0;
		// all_ate = 1;
		// while (i < info->count_philo)
		// {
		// 	pthread_mutex_lock(&(philo[i].ph_lock));
		// 	if (info->count_meals != -1
		// 		&& philo[i].meal_count < info->count_meals)
		// 	{
		// 		all_ate = 0;
		// 		pthread_mutex_unlock(&(philo[i].ph_lock));
		// 		break ;
		// 	}
		// 	pthread_mutex_unlock(&(philo[i].ph_lock));
		// 	i++;
		// }
		// if (all_ate == 1 && info->count_meals != -1)
		// 	return (1);
    return (0);
}

int is_dead(t_data *info, t_philo *philo)
{
	int i = 0;

	while (i < info->count_philo)
	{
		pthread_mutex_lock(&(philo[i].ph_lock));
		long long current_time = get_time();
		if ((current_time - philo[i].last_meal_time) >= info->time_to_die)
		{
			display_routine(info, i, "has died");
			pthread_mutex_lock(&(philo->info->lock));
			info->dead = 1;
			pthread_mutex_unlock(&(philo->info->lock));
			pthread_mutex_unlock(&(philo[i].ph_lock));
			return (1);
		}
		pthread_mutex_unlock(&(philo[i].ph_lock));
		i++;
	}
    return (0);
}

// int is_dead(t_data *info, t_philo *philo)
// {
//     int i = 0;
    
//     pthread_mutex_lock(&(info->lock)); // Lock the info lock

//     while (i < info->count_philo)
//     {
//         pthread_mutex_lock(&(philo[i].ph_lock)); // Lock the philosopher's lock

//         long long current_time = get_time();
//         long long last_meal_time = philo[i].last_meal_time; // Copy last_meal_time within lock
//         pthread_mutex_unlock(&(philo[i].ph_lock)); // Unlock philosopher's lock
        
//         if ((current_time - last_meal_time) >= info->time_to_die)
//         {
//             display_routine(info, i, "has died");
//             info->dead = 1; // Modify shared variable within lock
//             pthread_mutex_unlock(&(info->lock)); // Unlock the info lock
//             return (1);
//         }

//         i++;
//     }

//     pthread_mutex_unlock(&(info->lock)); // Unlock the info lock
//     return (0);
// }


int	simulation_finished(t_philo *philo, t_data *info)
{
	// int			i;
	// long long	current_time;
	// int			all_ate;

	// int total_meals = 0;
	// total_meals = info->count_meals * info->count_philo;
	while (2)
	{
		pthread_mutex_lock(&(philo->info->lock));
		if (info->dead)
		{
			pthread_mutex_unlock(&(philo->info->lock));
			break ;
		}
		// if ((info->count_meals != 0) && (total_meals <= 0))
		// {
		//     info->dead = 1;
		// 	pthread_mutex_unlock(&(philo->info->lock));
		//     break ;
		// }
		pthread_mutex_unlock(&(philo->info->lock));
		// i = 0;
		// all_ate = 1;
		// while (i < info->count_philo)
		// {
		// 	pthread_mutex_lock(&(philo[i].ph_lock));
		// 	if (info->count_meals != -1
		// 		&& philo[i].meal_count < info->count_meals)
		// 	{
		// 		all_ate = 0;
		// 		pthread_mutex_unlock(&(philo[i].ph_lock));
		// 		break ;
		// 	}
		// 	pthread_mutex_unlock(&(philo[i].ph_lock));
		// 	i++;
		// }
		// if (all_ate == 1 && info->count_meals != -1)
		// 	return (1);
		if (is_dead(info, philo) == 1)
			break ;
		if (finished_meals(info, philo) == 1)
			break ;
		// i = 0;
		// while (i < info->count_philo)
		// {
		// 	pthread_mutex_lock(&(philo[i].ph_lock));
		// 	current_time = get_time();
		// 	if ((current_time - philo[i].last_meal_time) >= info->time_to_die)
		// 	{
		// 		display_routine(info, i, "has died");
		// 		pthread_mutex_lock(&(philo->info->lock));
		// 		info->dead = 1;
		// 		pthread_mutex_unlock(&(philo->info->lock));
		// 		pthread_mutex_unlock(&(philo[i].ph_lock));
		// 		break ;
		// 	}
		// 	pthread_mutex_unlock(&(philo[i].ph_lock));
		// 	i++;
		// }
	}
	return (1);
}

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (philo->philo_id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&(philo->info->lock));
		if (philo->info->dead)
		{
			pthread_mutex_unlock(&(philo->info->lock));
			break ;
		}
		pthread_mutex_unlock(&(philo->info->lock));
		if (philo->info->count_meals > 0
			&& philo->meal_count >= philo->info->count_meals)
			return (NULL);
		is_eating(philo);
		if (philo->info->count_meals > 0
			&& philo->meal_count >= philo->info->count_meals)
			return (NULL);
		is_sleeping(philo);
		if (philo->info->count_meals > 0
			&& philo->meal_count >= philo->info->count_meals)
			return (NULL);
		is_thinking(philo);
	}
	return (NULL);
}

// void	run_simulation(t_philo *philo, t_data *info)
// {
// 	int	i;

// 	i = 0;
// 	while (i < info->count_philo)
// 	{
// 		pthread_create(&(philo[i].thread), NULL, philosopher, &philo[i]);
// 		usleep(100);
// 		i++;
// 	}
// 	simulation_finished(philo, info);
// 	i = 0;
// 	while (i < info->count_philo)
// 	{
// 		pthread_join(philo[i].thread, NULL);
// 		pthread_mutex_destroy(&(philo[i].ph_lock));
// 		i++;
// 	}
// 	pthread_mutex_destroy(&(info->lock));
// 	i = 0;
// 	while (i < info->count_philo)
// 	{
// 		pthread_mutex_destroy(&(info->forks[i]));
// 		i++;
// 	}
// 	free(info->forks);
// 	free(philo);
// }

int	main(int ac, char *av[])
{
	t_data	info;
	t_philo	*philo;

	if (parse_args(ac, av) == -1)
		return (1);
	memset(&info, 0, sizeof(t_data));
	get_data(&info, ac, av);
	initialize_forks(&info);
	initialize_philosophers(&philo, &info);
	run_simulation(philo, &info);
}