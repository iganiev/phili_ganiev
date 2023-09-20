/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganiev <iganiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:30:47 by iganiev           #+#    #+#             */
/*   Updated: 2023/09/20 21:14:19 by iganiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_space(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

long long	philo_atoi(const char *str)
{
	size_t		i;
	long		sign;
	long long	nbr;

	i = 0;
	nbr = 0;
	sign = 1;
	while (str[i] && is_space(str[i]))
		i++;
	if (str[i] == '-' && ++i)
		return (-1);
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == -1 && nbr * 10 < nbr)
			return (-1);
		if (sign == 1 && nbr * 10 < nbr)
			return (-1);
		nbr = (nbr * 10) + (str[i] - '0');
		i++;
	}
	return (nbr * sign);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec * 1000) + ((long long)tv.tv_usec / 1000));
}

void	ft_usleep(int ms)
{
	long long	start;

	start = get_time();
	usleep(ms * 900);
	while ((get_time() - start) < ms)
		usleep(100);
}

void	ft_free(t_philo *philo, t_data *info)
{
	free(info->forks);
	free(info->fork_state);
	free(philo);
}
