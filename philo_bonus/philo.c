/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:23:56 by andjenna          #+#    #+#             */
/*   Updated: 2024/11/30 20:24:26 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_sleep(t_philo *philo)
{
	ft_print_msg(philo, "is sleeping");
	// sem_wait(philo->prog->print);
	// printf("%d %d %s\n", get_time_ms() - philo->prog->start, philo->id, "is sleeping");
	// sem_post(philo->prog->print);
	ft_usleep(philo->prog->time_to_sleep, philo);
	return (0);
}

int	ft_think(t_philo *philo)
{
	int	time;

	ft_print_msg(philo, "is thinking");
	// sem_wait(philo->prog->print);
	// printf("%d %d %s\n", get_time_ms() - philo->prog->start, philo->id, "is thinking");
	// sem_post(philo->prog->print);
	if (philo->prog->time_to_eat > philo->prog->time_to_sleep)
	{
		time = philo->prog->time_to_eat - philo->prog->time_to_sleep + 1;
		ft_usleep(time, philo);
	}
	return (0);	
}

int	ft_eat(t_philo *philo)
{

	sem_wait(philo->prog->forks);
	if (ft_check_death(philo) == 1)
		return (1);
	// sem_wait(philo->prog->print);
	// printf("%d %d %s\n", get_time_ms() - philo->prog->start, philo->id, "has taken a fork");
	// sem_post(philo->prog->print);
	ft_print_msg(philo, "has taken a fork");
	sem_wait(philo->prog->forks);
	ft_print_msg(philo, "has taken a fork");
	// sem_wait(philo->prog->print);
	// printf("%d %d %s\n", get_time_ms() - philo->prog->start, philo->id, "has taken a fork");
	// sem_post(philo->prog->print);
	philo->last_meal = get_time_ms();
	sem_wait(philo->prog->print);
	printf("%s%d %d %s%s\n", YELLOW, get_time_ms() - philo->prog->start,
		philo->id, "is eating", RESET);
	sem_post(philo->prog->print);
	ft_usleep(philo->prog->time_to_eat, philo);
	sem_post(philo->prog->forks);
	sem_post(philo->prog->forks);
	philo->nb_eaten++;
	if (philo->prog->nb_time_to_eat != -1)
		philo->prog->has_eaten++;
	return (0);
}

int	ft_check_nb_eaten(t_philo *philo)
{
	if (philo->prog->nb_time_to_eat == -1)
		return (0);
	if (philo->prog->has_eaten == philo->prog->nb_time_to_eat)
	{
		ft_clean_sem(philo->prog);
		exit(0);
	}
	return (0);
}

int	ft_check_death(t_philo *philo)
{
	if (get_time_ms() - philo->last_meal >= philo->prog->time_to_die)
	{
		sem_post(philo->prog->death);
		ft_clean_sem(philo->prog);
		exit(0);
	}
	return (0);
}



void	ft_routine(t_philo *philo)
{
	while (!ft_check_death(philo) && !ft_check_nb_eaten(philo))
	{
		if (philo->prog->nb_of_philo == 1)
		{
			sem_wait(philo->prog->forks);
			ft_print_msg(philo, "has taken a fork");
			philo->last_meal = get_time_ms();
			while (!ft_check_death(philo))
				;
		}
		if (philo->id % 2 == 0)
			usleep(1500);
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	exit(0);
}