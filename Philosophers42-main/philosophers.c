/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iganiev <iganiev@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:30:37 by iganiev           #+#    #+#             */
/*   Updated: 2023/09/20 21:19:35 by iganiev          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
