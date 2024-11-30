/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:36:10 by andjenna          #+#    #+#             */
/*   Updated: 2024/11/12 11:20:35 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_sleep(t_philo *philo)
{
	ft_print_msg(philo, "is sleeping");
	ft_usleep(philo->prog->time_to_sleep, philo);
}

static void	ft_eat(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->l_fork);
		ft_print_msg(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
		ft_print_msg(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		ft_print_msg(philo, "has taken a fork");
		pthread_mutex_lock(&philo->l_fork);
		ft_print_msg(philo, "has taken a fork");
	}
	ft_print_msg(philo, "is eating");
	philo->last_meal = get_time_ms();
	philo->nb_eaten = philo->nb_eaten + 1;
	ft_usleep(philo->prog->time_to_eat, philo);
	pthread_mutex_unlock(&philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

static void	ft_think(t_philo *philo)
{
	int	time_to_think;

	pthread_mutex_lock(&philo->prog->data);
	time_to_think = (philo->prog->time_to_eat - philo->prog->time_to_sleep) + 1;
	pthread_mutex_unlock(&philo->prog->data);
	ft_print_msg(philo, "is thinking");
	ft_usleep(time_to_think, philo);
}

void	*ft_routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (!ft_check_death(philo) && !ft_check_has_eaten(philo))
	{
		if (philo->id % 2 == 0)
			usleep(1500);
		if (get_value(&philo->prog->nb_of_philo, &philo->prog->data) == 1)
		{
			pthread_mutex_lock(&philo->l_fork);
			ft_print_msg(philo, "has taken a fork");
			while (!ft_check_death(philo))
				;
			pthread_mutex_unlock(&philo->l_fork);
			break ;
		}
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	return (NULL);
}